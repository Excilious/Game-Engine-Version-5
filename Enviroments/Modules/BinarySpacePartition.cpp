#include "BinarySpacePartition.h"
#include "../Common/Console.h"
#include "../Rendering/Texture.h"

BinarySpacePartition::BinarySpacePartition(void): FilePointer(NULL), BinaryRecursion(0), FaceBitSet(0), FaceBitSize(0)
{
    this->NumberOfVerts             = 0;
    this->NumberOfFaces             = 0;
    this->NumberOfIndices           = 0;
    this->NumberOfTextures          = 0;
    this->NumberOfLightmaps         = 0;
    this->NumberOfVisibleFaces      = 0;
    this->SkipIndices               = 0;

    this->Vertices                  = NULL;
    this->Indices                   = NULL;
    this->Faces                     = NULL;
    memset(&this->VisibleData,0,sizeof(BSPVisData));
}

glm::vec3 BinarySpacePartition::SpawnPlayerInBSP()
{return this->PlayerSpawnPositions[rand()%this->PlayerSpawnPositions.size()];}

std::vector<glm::vec3> BinarySpacePartition::GetEntityLights(const std::string& EntityText)
{
    std::vector<glm::vec3> NewLightPositions;
    std::regex EntityRegex(R"(\{[^]*\})");
    std::regex ClassnameRegex(R"("classname"\s+"light")");
    std::regex OriginRegex("\"origin\"\\s*\"([^\"]+)\"");

    auto EntityBegin    = std::sregex_iterator(EntityText.begin(),EntityText.end(),EntityRegex);
    auto EntityEnd      = std::sregex_iterator();

    for (auto Index = EntityBegin; Index != EntityEnd; Index++)
    {
        std::string Block = Index->str();
        if (std::regex_search(Block,ClassnameRegex))
        {
            std::smatch OriginMatch;
            if (std::regex_search(Block,OriginMatch,OriginRegex))
            {
                glm::vec3 Position;
                std::istringstream StringStream(OriginMatch[1].str());
                StringStream >> Position.x >> Position.y >> Position.z;

                Position.y  = fabsf(Position.y);
                Position.z  = fabsf(Position.z);      
                Position.y  = (unsigned int)Position.y^(unsigned int)Position.z;
                Position.z  = (unsigned int)Position.z^(unsigned int)Position.y;
                Position.y  = (unsigned int)Position.y^(unsigned int)Position.z;
                Position.z *= -1;

                NewLightPositions.push_back(Position);
            }
        }
    }
    return LightPositions;
}

int BinarySpacePartition::IsClusterVisible(int Current,int Test)
{
    if (this->VisibleData.BitSets || Current < 0) {return 1;}

    char VisibleSet = this->VisibleData.BitSets[(Current*this->VisibleData.CharacterPerCluster)+(Test/8)];
    int Result      = VisibleSet&(1<<((Test)&7));
    return (Result);
}

int BinarySpacePartition::FindLeaf(glm::vec3 Point)
{
    float Temporary = Point.y;
    Point.y = Point.z;
    Point.z = -Temporary;
    int Index       = 0;
    float Distance  = 0.0f;

    while (Index >= 0)
    {
        const BSPNode& NewNode      = this->SceneNode[Index];
        const BSPPlane& NewPlane    = this->ScenePlane[NewNode.PlaneIndex];
        Distance                    = NewPlane.Normal.x*Point.x+NewPlane.Normal.y*Point.y+NewPlane.Normal.z*Point.z-NewPlane.Distance;
        
        if (Distance >= 0)  {Index = NewNode.Front;}
        else                {Index = NewNode.Back;}
    }
    return ~Index;
}

void BinarySpacePartition::SetupBinarySpacePartition(Parser* EngineParser)
{
    this->BuildVertexBuffer();
    this->GenerateTextures(EngineParser);
    this->GenerateLightMaps();
}

void BinarySpacePartition::GetPlayerSpawnPositions(void)
{
    if (this->FilePointer == NULL) {SysPrint("Failed to get position: File has been closed.",2);}
    int EntityLength = this->Lumps[KEntites].Length;
    int EntityOffset = this->Lumps[KEntites].Offset;
    size_t Position  = 0;

    char* EntityData = new char[EntityLength+1];
    fseek(this->FilePointer,EntityOffset,SEEK_SET);
    fread(EntityData,1,EntityLength,this->FilePointer);
    EntityData[EntityLength] = '\0';

    std::string EntityText(EntityData);
    delete[] EntityData;

    while ((Position = EntityText.find("{",Position)) != std::string::npos)
    {
        size_t EndPosition = EntityText.find("}",Position);
        if (EndPosition == std::string::npos) {break;}

        std::string EntityBlock = EntityText.substr(Position,EndPosition-Position);
        if (EntityBlock.find("\"classname\" \"info_player_deathmatch\"") != std::string::npos || EntityBlock.find("\"classname\" \"info_player_start\"") != std::string::npos)
        {
            size_t OriginKey = EntityBlock.find("\"origin\"");
            if (OriginKey != std::string::npos)
            {
                size_t FirstValueQuote = EntityBlock.find("\"",OriginKey+8);
                if (FirstValueQuote != std::string::npos)
                {
                    size_t SecondValueQuote = EntityBlock.find("\"",FirstValueQuote+1);
                    if (SecondValueQuote != std::string::npos)
                    {
                        std::string NewOriginString = EntityBlock.substr(FirstValueQuote+1,SecondValueQuote-FirstValueQuote-1);
                        float X,Y,Z;
                        if (sscanf(NewOriginString.c_str(),"%f %f %f",&X,&Y,&Z) == 3)
                        {
                            Y = fabsf(Y); Z = fabsf(Z);
                            Y = (unsigned int)Y^(unsigned int)Z;
                            Z = (unsigned int)Z^(unsigned int)Y;
                            Y = (unsigned int)Y^(unsigned int)Z;
                            Z*= -1;

                            this->PlayerSpawnPositions.push_back(glm::vec3(X,Y,Z));
                        }
                        else {SysPrint("Failed to parse origin on: "+NewOriginString, 2);}
                    }
                }
            }
        }
        Position = EndPosition + 1;
    }
}

void BinarySpacePartition::LoadBinarySpacePartition(std::string FileName,Shader* BinaryShader)
{
    SysPrint("Using "+FileName,1);
    if ((this->FilePointer = fopen(FileName.c_str(),"rb")) == NULL)
    {SysPrint("Cannot open BSP File",2); return;}

    fread(&this->Header,1,sizeof(BSPHeader),this->FilePointer);
    fread(&this->Lumps,KMaxLumps,sizeof(BSPLump),this->FilePointer);

    if (this->Header.Version != 0x2e)
    {SysPrint("Failed to load bsp. Not an IBSP.",2); return;}

    this->NumberOfNodes     = this->Lumps[KNodes].Length/sizeof(BSPNode);
    this->SceneNode         = new BSPNode[this->NumberOfNodes];

    fseek(this->FilePointer,this->Lumps[KNodes].Offset,SEEK_SET);
    fread(this->SceneNode,this->NumberOfNodes,sizeof(BSPNode),this->FilePointer);

    this->NumberOfVerts     = this->Lumps[KVertices].Length/sizeof(BSPVertex);
    this->Vertices          = new BSPVertex[this->NumberOfVerts];

    this->NumberOfLeafs     = this->Lumps[KLeafs].Length/sizeof(BSPLeaf);
    this->SceneLeaf         = new BSPLeaf[this->NumberOfLeafs];

    fseek(this->FilePointer,this->Lumps[KLeafs].Offset,SEEK_SET);
    fread(this->SceneLeaf,this->NumberOfLeafs,sizeof(BSPLeaf),this->FilePointer);

    this->NumberOfFaces     = this->Lumps[KFaces].Length/sizeof(BSPFace);
    this->Faces             = new BSPFace[this->NumberOfFaces];

    this->NumberOfIndices   = this->Lumps[KIndices].Length/sizeof(int);
    this->Indices           = new int[this->NumberOfIndices];

    this->NumberOfLeafFace  = this->Lumps[KLeafFaces].Length/sizeof(int);
	this->LeafFace          = new int [this->NumberOfLeafFace];

    fseek(this->FilePointer,this->Lumps[KLeafFaces].Offset,SEEK_SET);
    fread(this->LeafFace,this->NumberOfLeafFace,sizeof(int),this->FilePointer);

    this->NumberOfPlanes    = this->Lumps[KPlanes].Length/sizeof(BSPPlane);
    this->ScenePlane        = new BSPPlane[this->NumberOfPlanes];

    this->NumberOfTextures  = this->Lumps[KTextures].Length/sizeof(BSPTexture);
    fseek(this->FilePointer,this->Lumps[KVertices].Offset,SEEK_SET);

    for (int Index = 0; Index < this->NumberOfVerts; Index++)
    {
        fread(&this->Vertices[Index],1,sizeof(BSPVertex),this->FilePointer);
        float Temporary                     = this->Vertices[Index].Position.y;
        this->Vertices[Index].Position.y    = this->Vertices[Index].Position.z;
        this->Vertices[Index].Position.z    = -Temporary; 
    }

    this->GetPlayerSpawnPositions();
    fseek(this->FilePointer,this->Lumps[0].Offset,SEEK_SET);
    char* EntityData = new char[this->Lumps[0].Length+1];
    fread(EntityData,1,this->Lumps[0].Length,this->FilePointer);
    EntityData[this->Lumps[0].Length] = '\0';

    std::string EntityString(EntityData);
    this->LightPositions = this->GetEntityLights(EntityString);
    delete EntityData;

    BSPTexture* Textures = new BSPTexture[this->NumberOfTextures];
    fseek(this->FilePointer,this->Lumps[KIndices].Offset,SEEK_SET);
    fread(this->Indices,this->NumberOfIndices,sizeof(int),this->FilePointer);
    fseek(this->FilePointer,this->Lumps[KFaces].Offset,SEEK_SET);
    fread(this->Faces,this->NumberOfFaces,sizeof(BSPFace),this->FilePointer);
    fseek(this->FilePointer,this->Lumps[KTextures].Offset,SEEK_SET);
    fread(Textures,this->NumberOfTextures,sizeof(BSPTexture),this->FilePointer);

    fseek(this->FilePointer,this->Lumps[KVisData].Offset,SEEK_SET);
    if (this->Lumps[KVisData].Length)
    {
        fread(&(this->VisibleData.NumberOfClusters),1,sizeof(int),this->FilePointer);
        fread(&(this->VisibleData.CharacterPerCluster),1,sizeof(int),this->FilePointer);

        int VisClusterSize = this->VisibleData.NumberOfClusters*this->VisibleData.CharacterPerCluster;
        this->VisibleData.BitSets = new char [VisClusterSize];

        fread(this->VisibleData.BitSets,1,sizeof(char)*VisClusterSize,this->FilePointer);
    }
    else
    {this->VisibleData.BitSets = NULL;}

    for (int Index = 0; Index < this->NumberOfTextures; Index++)
    {
        strcpy(this->TNameCache[Index],Textures[Index].StringName);
        std::string FileLocation = std::get<std::string>(GetFromRegister("location"))+"/";
        if (std::filesystem::exists((std::string)FileLocation+this->TNameCache[Index]+(std::string)".tga"))           {strcat(this->TNameCache[Index],".tga");}
        else if (std::filesystem::exists((std::string)FileLocation+this->TNameCache[Index]+(std::string)".jpg"))      {strcat(this->TNameCache[Index],".jpg");}

        this->TName.push_back(this->TNameCache[Index]);
    }

    this->NumberOfLightmaps = this->Lumps[KLightmaps].Length/sizeof(BSPLightmap);
    BSPLightmap* LightMaps  = new BSPLightmap[this->NumberOfLightmaps];
    fseek(this->FilePointer,this->Lumps[KLightmaps].Offset,SEEK_SET);

    for (int Index = 0; Index < this->NumberOfLightmaps; Index++)
    {
        fread(&LightMaps[Index],1,sizeof(BSPLightmap),this->FilePointer);
        this->RendBuffer.LightMaps.push_back(LightMaps[Index]);
    }

    this->FaceBitSize   = (this->NumberOfFaces)/32+1;
    if (this->FaceBitSet) {delete this->FaceBitSet; this->FaceBitSet = 0;}
    this->FaceBitSet    = new unsigned int[this->FaceBitSize];  
    memset(this->FaceBitSet,0,sizeof(unsigned int)*this->FaceBitSize);

    delete[] Textures;
    delete[] LightMaps;

    fclose(this->FilePointer);
}

void BinarySpacePartition::BuildVertexBuffer(void)
{
    for (int Index = 0; Index < this->NumberOfFaces; Index++)
    {
        BSPFace* NewFace = &this->Faces[Index];
        if (NewFace->Type == FACE_POLYGON) {this->SkipIndices += NewFace->NumberOfIndices;}

        this->CreateVertexBuffer(Index);
        this->CreateIndices(Index);
        this->CreateRenderBuffers(Index);
        this->NumberOfVisibleFaces++;
    }
    for (int Index = 0; Index < this->LightPositions.size(); Index++)
    {this->NewEnviromentLighting.CreateLight(glm::vec3(1.0f,0.0f,0.0f),this->LightPositions[Index],0.5f);}
}

void BinarySpacePartition::CreateVertexBuffer(int Index)
{
    BSPFace* NewFace = &this->Faces[Index];

    for (int VertexIndex = 0; VertexIndex < NewFace->NumberOfVertex; VertexIndex++)
    {
        this->RendBuffer.FaceVertexBuffers[Index].push_back(this->Vertices[NewFace->StartVertexIndex+VertexIndex].Position.x);
        this->RendBuffer.FaceVertexBuffers[Index].push_back(this->Vertices[NewFace->StartVertexIndex+VertexIndex].Position.y);
        this->RendBuffer.FaceVertexBuffers[Index].push_back(this->Vertices[NewFace->StartVertexIndex+VertexIndex].Position.z);
        this->RendBuffer.FaceVertexBuffers[Index].push_back(this->Vertices[NewFace->StartVertexIndex+VertexIndex].TextureCoord.x);
        this->RendBuffer.FaceVertexBuffers[Index].push_back(this->Vertices[NewFace->StartVertexIndex+VertexIndex].TextureCoord.y);
        this->RendBuffer.FaceVertexBuffers[Index].push_back(this->Vertices[NewFace->StartVertexIndex+VertexIndex].LightMapCoord.x);
        this->RendBuffer.FaceVertexBuffers[Index].push_back(this->Vertices[NewFace->StartVertexIndex+VertexIndex].LightMapCoord.y);
    }
}

void BinarySpacePartition::CreateIndices(int Index)
{
    BSPFace* NewFace    = &this->Faces[Index];
    int IndexOffset     = NewFace->StartIndex;
    for (int IndicesIndex = 0; IndicesIndex < NewFace->NumberOfIndices; IndicesIndex++)
    {this->RendBuffer.FaceVertexIndex[Index].push_back(this->Indices[IndexOffset+IndicesIndex]);}
}

void BinarySpacePartition::Debug(int Index)
{
    SysPrint("Face:---->"+std::to_string(Index),2);
    for (unsigned int NewIndex = 0; NewIndex < this->RendBuffer.FaceVertexBuffers[Index].size(); NewIndex++)
    {SysPrint(">-"+std::to_string(this->RendBuffer.FaceVertexBuffers[Index][NewIndex]),2);}

    for (unsigned int NewIndex = 0; NewIndex < this->RendBuffer.FaceVertexBuffers[Index].size(); NewIndex++)
    {SysPrint(">-"+std::to_string(this->RendBuffer.FaceVertexIndex[Index][NewIndex]),2);}

    SysPrint("Vbuffer"+std::to_string(sizeof(GLfloat)*this->RendBuffer.FaceVertexBuffers[Index].size()),2);
    SysPrint("Ibuffer"+std::to_string(sizeof(GLuint)*this->RendBuffer.FaceVertexIndex[Index].size()),2);
    SysPrint("Endface",2);
}

void BinarySpacePartition::CreateRenderBuffers(int Index)
{
    glGenVertexArrays(1,&(this->FaceBufferArray.FaceBufferIndex[Index].VertexArray));
    glBindVertexArray((this->FaceBufferArray.FaceBufferIndex[Index].VertexArray));

    glGenBuffers(1,&(this->FaceBufferArray.FaceBufferIndex[Index].VertexBuffers));
    glBindBuffer(GL_ARRAY_BUFFER,this->FaceBufferArray.FaceBufferIndex[Index].VertexBuffers);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*this->RendBuffer.FaceVertexBuffers[Index].size(),&this->RendBuffer.FaceVertexBuffers[Index].front(),GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,7*sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,7*sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,7*sizeof(GLfloat),(GLvoid*)(5*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
}

void BinarySpacePartition::GenerateTextures(Parser* EngineParser)
{
    GLuint TextureIDs;
    GLfloat Anisope         = 8.0f;
    int Width,Height;
    int NumberOfChannels    = 3;
    GLuint MissingIds       = 1;
    
    glGenTextures(1,&MissingIds);
    unsigned char* Image = EngineParser->GetGlobalImageBuffer("missing.imgbuf");

    glBindTexture(GL_TEXTURE_2D,MissingIds);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,1024,1024,0,GL_RGBA,GL_UNSIGNED_BYTE,Image);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(Image);

    for (int Index = 0; Index < this->NumberOfFaces; Index++)
    {
        BSPFace* NewFace                            = &this->Faces[Index];
        this->RendBuffer.TextureOptimised[Index]    = NewFace->TextureID+3;
    }

    std::vector<GLuint> MissingTextureIds;
    for (int Index = 0; Index < this->NumberOfTextures; Index++)
    {
        unsigned char* Image = stbi_load((std::get<std::string>(GetFromRegister("location"))+"/"+this->TName[Index]).c_str(),&Width,&Height,&NumberOfChannels,3);
        if (Image)
        {
            glGenTextures(1,&TextureIDs);
            glBindTexture(GL_TEXTURE_2D,TextureIDs);

            glGetFloatv(GL_ANISOPE,&Anisope);
            glTexParameterf(GL_TEXTURE_2D,GL_ANISOPE,Anisope);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);

            glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,Width,Height,0,GL_RGB,GL_UNSIGNED_BYTE,Image);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(Image);
        }
        else
        {
            SysPrint("Failed to load "+this->TName[Index],2);
            stbi_image_free(Image);
            glGenTextures(1,&TextureIDs);
            MissingTextureIds.push_back(TextureIDs);
            glBindTexture(GL_TEXTURE_2D,MissingIds);
        }
    }

    for (GLuint Index = 0; Index < this->RendBuffer.TextureOptimised.size(); Index++)
    {
        for (auto Value: MissingTextureIds)
        {
            if (this->RendBuffer.TextureOptimised[Index] == Value)
            {this->RendBuffer.TextureOptimised[Index] = MissingIds;}
        }
    }
}

void BinarySpacePartition::GenerateLightMaps(void)
{
    GLfloat WhiteLightMap[] ={
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    };

    glGenTextures(1,&MissingLightmapIDs);
    glBindTexture(GL_TEXTURE_2D,MissingLightmapIDs);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,2,2,0,GL_RGB,GL_FLOAT,&WhiteLightMap);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    this->LightmapTexturesID = new GLuint[this->NumberOfLightmaps];
    glGenTextures(this->RendBuffer.LightMaps.size(),this->LightmapTexturesID);

    for (GLuint Index = 0; Index < this->RendBuffer.LightMaps.size(); Index++)
    {
        glBindTexture(GL_TEXTURE_2D,this->LightmapTexturesID[Index]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);

        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,128,128,0,GL_RGB,GL_UNSIGNED_BYTE,&this->RendBuffer.LightMaps[Index].ImageBits);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}


void BinarySpacePartition::RenderOneFace(int Index,unsigned int Program,glm::mat4 ViewMatrix,glm::mat4 ProjectionMatrix)
{
    glBindVertexArray(this->FaceBufferArray.FaceBufferIndex[Index].VertexArray);
    BSPFace* NewFace  = &this->Faces[Index];
    GLuint TextureID  = this->RendBuffer.TextureOptimised[Index];
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,TextureID);

    glActiveTexture(GL_TEXTURE1);
    if (NewFace->LightMapID >= 0)   {glBindTexture(GL_TEXTURE_2D,this->LightmapTexturesID[NewFace->LightMapID]);}
    else                            {glBindTexture(GL_TEXTURE_2D,this->MissingLightmapIDs);}

    glUniformMatrix4fv(glGetUniformLocation(Program,"projection"),1,GL_FALSE,glm::value_ptr(glm::mat4(ProjectionMatrix)));
    glUniformMatrix4fv(glGetUniformLocation(Program,"view"),1,GL_FALSE,glm::value_ptr(glm::mat4(ViewMatrix)));
    glUniform1i(glGetUniformLocation(Program,"s_bspTexture"),0);
    glUniform1i(glGetUniformLocation(Program,"s_bspLightmap"),1);
    glUniform1f(glGetUniformLocation(Program,"ApplyLightMap"),std::get<float>(GetFromRegister("ApplyLightMap")));
    glUniform1f(glGetUniformLocation(Program,"LightIntensity"),std::get<float>(GetFromRegister("LightIntensity")));
    glUseProgram(Program);
    glDrawElements(GL_TRIANGLES,NewFace->NumberOfIndices,GL_UNSIGNED_INT,&this->Indices[NewFace->StartIndex]);
    glBindVertexArray(0);
}

void BinarySpacePartition::RenderAllFaces(Shader* BinaryShader,const glm::vec3 CameraPosition,glm::mat4 ViewMatrix,glm::mat4 ProjectionMatrix)
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    this->NewEnviromentLighting.ResolveBinarySpaceLighting(BinaryShader,this->LightPositions,CameraPosition);
    memset(this->FaceBitSet,0,sizeof(unsigned int)*this->FaceBitSize);
    int LeafIndex   = this->FindLeaf(CameraPosition);
    int Cluster     = this->SceneLeaf[LeafIndex].Cluster;
    int LeafCount   = this->NumberOfLeafs;

    while (LeafCount--)
    {
        BSPLeaf *NewLeaf = &(this->SceneLeaf[LeafCount]);
        if (!this->IsClusterVisible(Cluster,NewLeaf->Cluster)) {continue;}

        int FaceCount = NewLeaf->NumLeafFace;
        while (FaceCount--)
        {
            int FaceIndex = this->LeafFace[NewLeaf->FirstLeafFace+FaceCount];
            if (this->Faces[FaceIndex].Type == 2 || this->Faces[FaceIndex].Type == 4) {continue;}
            
            if (!(this->FaceBitSet[FaceIndex>>5]&(1<<(FaceIndex&31))))
            {
                this->RenderOneFace(FaceIndex,BinaryShader->Program,ViewMatrix,ProjectionMatrix);
                this->FaceBitSet[FaceIndex>>5] |= (1<<(FaceIndex&31));
            }
        }
    }
}

void BinarySpacePartition::DestroyScene()
{
    if (this->Vertices)                     {delete[] this->Vertices; this->Vertices = NULL;}
    if (this->Faces)                        {delete[] this->Faces; this->Faces = NULL;}
    if (this->Indices)                      {delete[] this->Indices; this->Indices = NULL;}
    if (this->VisibleData.BitSets)          {delete[] this->VisibleData.BitSets; this->VisibleData.BitSets = NULL;}

    for (int Index = 0; Index < this->NumberOfFaces; Index++) {glDeleteVertexArrays(1,&(this->FaceBufferArray.FaceBufferIndex[Index].VertexArray));}
    for (int Index = 0; Index < this->NumberOfFaces; Index++) {glDeleteBuffers(1,&(this->FaceBufferArray.FaceBufferIndex[Index].VertexBuffers));}
}