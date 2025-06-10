#define STB_PERLIN_IMPLEMENTATION
#include "Terrain.h"
#include "../../Common/Console.h"

Terrain::Terrain(void) {}

glm::vec3 Terrain::GetNextPlayerPosition(glm::vec3 PlayerPosition)
{return glm::vec3(0,-this->TerrainHeightMap[abs((int)PlayerPosition.x)][abs((int)PlayerPosition.z)]+4.5,0);}

void Terrain::GenerateTerrainMesh(Parser* EngineParser)
{
    SysPrint("Generating Terrain...",1);
    //this->TerrainTexture->CreateBufferTexture(EngineParser->GetImageBuffer("Terrain.imgbuf"),1024,1024,4);
    this->TerrainTexture.CreateNormalTexture("../../Textures/TextureTerrain.png","diffuse");
    this->TerrainShader.CreateNewShader(EngineParser->GetGlobalDatapackData("Terrain.vert").c_str(),EngineParser->GetGlobalDatapackData("Terrain.frag").c_str(),true);
    this->TerrainVertices.clear();
    this->TerrainIndices.clear();

    for (int X = 0; X < TERRAIN_MAP; X++)
    {
        for (int Z = 0; Z < TERRAIN_MAP; Z++)
        {
            float Y = this->TerrainHeightMap[X][Z];
            TerrainVertex Vertex;
            Vertex.Position = glm::vec3(X,Y,Z);
            Vertex.Normal   = glm::vec3(0.0f,1.0f,0.0f);
            Vertex.UV       = glm::vec2((float)X/TERRAIN_MAP*10,(float)Z/TERRAIN_MAP*10);
            this->TerrainVertices.push_back(Vertex);
        }
    }

    for (int X = 0; X < TERRAIN_MAP-1; X++)
    {
        for (int Z = 0; Z < TERRAIN_MAP-1; Z++)
        {
            int TopLeft     = X*TERRAIN_MAP+Z;
            int TopRight    = (X+1)*TERRAIN_MAP+Z;
            int BottomLeft  = X*TERRAIN_MAP+(Z+1);
            int BottomRight = (X+1)*TERRAIN_MAP+(Z+1);


            this->TerrainIndices.push_back(TopLeft);
            this->TerrainIndices.push_back(BottomLeft);
            this->TerrainIndices.push_back(TopRight);
            this->TerrainIndices.push_back(TopRight);
            this->TerrainIndices.push_back(BottomLeft);
            this->TerrainIndices.push_back(BottomRight);
        }
    }
    SysPrint("Generated Terrain. ("+std::to_string(this->TerrainVertices.size())+" bytes for vertices and "+std::to_string(this->TerrainIndices.size())+" bytes for indices.)",1);
}

void Terrain::GenerateTerrainBuffers(void)
{
    glGenVertexArrays(1,&this->VertexArrayObject);
    glGenBuffers(1,&this->VertexBufferObject);
    glGenBuffers(1,&this->ElementBufferObject);

    glBindVertexArray(this->VertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER,this->VertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER,this->TerrainVertices.size()*sizeof(TerrainVertex),this->TerrainVertices.data(),GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->ElementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,this->TerrainIndices.size()*sizeof(unsigned int),this->TerrainIndices.data(),GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(TerrainVertex),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(TerrainVertex),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(TerrainVertex),(void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    
    glm::mat4 TranslationMatrix = glm::mat4(1.0f);
    glm::mat4 RotationMatrix    = glm::mat4(1.0f);
    glm::mat4 ScaleMatrix       = glm::mat4(1.0f);
    
    TranslationMatrix           = glm::translate(TranslationMatrix,glm::vec3(100.0f,7.0f,-40.0f));
    RotationMatrix              = glm::mat4_cast(glm::quat(0.0f,0.0f,0.0f,1.0f));
    ScaleMatrix                 = glm::scale(ScaleMatrix,glm::vec3(1.0f,1.0f,1.0f));
    this->ModelMatrix           = TranslationMatrix*RotationMatrix*ScaleMatrix;

    glBindVertexArray(0);
}

void Terrain::RenderTerrain(Camera* EngineCamera)
{
    this->TerrainShader.ActivateShader();
    glUniformMatrix4fv(glGetUniformLocation(this->TerrainShader.Program,"model"),1,GL_FALSE,glm::value_ptr(this->ModelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(this->TerrainShader.Program,"view"),1,GL_FALSE,glm::value_ptr(EngineCamera->ViewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(this->TerrainShader.Program,"projection"),1,GL_FALSE,glm::value_ptr(EngineCamera->ProjectionMatrix));

    glUniform3f(glGetUniformLocation(this->TerrainShader.Program,"lightPos"),EngineCamera->Position.x,EngineCamera->Position.y,EngineCamera->Position.z);
    glUniform3f(glGetUniformLocation(this->TerrainShader.Program,"lightColor"),0.0f,1.0f,0.0f);
    glUniform3f(glGetUniformLocation(this->TerrainShader.Program,"viewPos"),EngineCamera->Position.x,EngineCamera->Position.y,EngineCamera->Position.z);
    this->TerrainTexture.Bind();
    this->TerrainTexture.TextureUnit(&this->TerrainShader,"Terrain0",0);

    glBindVertexArray(this->VertexArrayObject);
    glDrawElements(GL_TRIANGLES,this->TerrainIndices.size(),GL_UNSIGNED_INT,0);
    glBindVertexArray(0);
}

void Terrain::GenerateHeightMap(void)
{
    for (int IndexX = 0; IndexX < TERRAIN_MAP; IndexX++)
    {
        for (int IndexZ = 0; IndexZ < TERRAIN_MAP; IndexZ++)
        {
            this->NoiseValue    = 0.0f;
            this->Amplitude     = 1.0f;
            this->Frequency     = 1.0f;

            for (int Index = 0; Index < this->Octaves; Index++)
            {
                this->NoiseValue += this->Amplitude*stb_perlin_noise3(IndexX*this->Scale*this->Frequency,0.0f,IndexZ*this->Scale*this->Frequency,0,0,0);
                this->Amplitude  *= this->Persistance;
                this->Frequency  *= this->Lacunarity;   
            }
            this->NoiseValue = (this->NoiseValue+1.0f)/2.0f;
            this->TerrainHeightMap[IndexX][IndexZ] = this->NoiseValue * this->HeightMultiplier;
        }
    }
}

void Terrain::Destroy(void)
{
    glDeleteVertexArrays(1,&this->VertexArrayObject);
    this->TerrainTexture.Delete();
    this->TerrainShader.DestroyShader();
}