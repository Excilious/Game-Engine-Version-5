#include "Model.h"

Model::Model(void) {}

void Model::LoadFromGLTF(Parser* EngineParser,std::string MeshSource,glm::vec3 Translation,glm::quat Rotation,glm::vec3 Scale,std::string ImageTexture,glm::vec2 TextureSize)
{
    this->ModelVertexArray.CreateVertexArray();
    this->LoadMeshUsingGLTF(EngineParser,MeshSource);
    if (this->MeshVertices.size() != 0 && this->MeshIndices.size() != 0)
    {
        this->ModelTextureData      = EngineParser->GetGlobalImageBuffer(ImageTexture);
        this->TranslationMatrix     = glm::mat4(1.0f);
        this->RotationMatrix        = glm::mat4(1.0f);
        this->ScaleMatrix           = glm::mat4(1.0f);
        this->ModelPosition         = Translation;
        this->ModelRotation         = Rotation;
        this->ModelScale            = Scale;

        if (this->ModelTextureData == (unsigned char*)"") {SysPrint("Failed to load model. Invalid texture.",3); return;}
        this->TranslationMatrix     = glm::translate(this->TranslationMatrix,this->ModelPosition);
        this->RotationMatrix        = glm::mat4_cast(this->ModelRotation);
        this->ScaleMatrix           = glm::scale(this->ScaleMatrix,this->ModelScale);
        this->Vertices              = this->FloatPositionVectors;
        this->Indices               = MeshIndices;
        this->SetModelTexture(TextureSize);

        this->ModelVertexArray.Bind();
        VertexBufferObject ModelVertexBuffer(this->MeshVertices);
        ElementBufferObject ModelElementBuffer(this->MeshIndices);

        this->ModelVertexArray.LinkAttribute(ModelVertexBuffer,0,3,GL_FLOAT,sizeof(SceneVertex),(void*)0);
        this->ModelVertexArray.LinkAttribute(ModelVertexBuffer,1,3,GL_FLOAT,sizeof(SceneVertex),(void*)(3*sizeof(float)));
        this->ModelVertexArray.LinkAttribute(ModelVertexBuffer,2,3,GL_FLOAT,sizeof(SceneVertex),(void*)(6*sizeof(float)));
        this->ModelVertexArray.LinkAttribute(ModelVertexBuffer,3,2,GL_FLOAT,sizeof(SceneVertex),(void*)(9*sizeof(float)));

        this->ModelVertexArray.Unbind();
        ModelVertexBuffer.Unbind();
        ModelElementBuffer.Unbind();
    }
    else
    {SysPrint("Failed to load "+MeshSource+" model asset.",2);}
}

void Model::UpdateMatrix(glm::vec3 Position,glm::quat Rotation,glm::vec3 Scale)
{ 
    if (Position != glm::vec3(0.0f,0.0f,0.0f))      
    {
        this->ModelPosition     = Position;
        this->TranslationMatrix = glm::translate(this->TranslationMatrix,this->ModelPosition);
    }
    if (Rotation != glm::quat(0.0f,0.0f,0.0f,0.0f)) 
    {
        this->ModelRotation     = Rotation;
        this->RotationMatrix    = glm::mat4_cast(this->ModelRotation);
    }
    if (Scale != glm::vec3(0.0f,0.0f,0.0f))         
    {
        this->ModelScale        = Scale;
        this->ScaleMatrix       = glm::scale(this->ScaleMatrix,this->ModelScale);
    }
}

void Model::SetModelTexture(glm::vec2 TextureSize)
{
    glGenTextures(1,&this->ModelTextureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,this->ModelTextureID);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,TextureSize.x,TextureSize.y,0,GL_RGBA,GL_UNSIGNED_BYTE,this->ModelTextureData);
    glGenerateMipmap(GL_TEXTURE_2D);
    free(this->ModelTextureData);
    glBindTexture(GL_TEXTURE_2D,0);
}

void Model::DrawModel(Shader* ShaderInstance,Camera* CameraInstance,float NearPlane,float FarPlane,float FieldOfView,glm::mat4 CameraMatrix)
{
    this->ModelVertexArray.Bind();

    ShaderInstance->ActivateShader();    
    CameraInstance->UpdateMatrix(FieldOfView,NearPlane,FarPlane);
    ShaderInstance->SetMatrix4(CameraMatrix,"CameraMatrix");
    ShaderInstance->SetVector3(CameraInstance->Position,"CameraPosition");
    ShaderInstance->SetVector3(glm::vec3(1.0f,0.0f,0.0f),"Colour");
    ShaderInstance->SetMatrix4(this->TranslationMatrix,"Translation");
    ShaderInstance->SetMatrix4(this->RotationMatrix,"Rotation");
    ShaderInstance->SetMatrix4(this->ScaleMatrix,"Scale");

    glUniform1i(glGetUniformLocation(ShaderInstance->Program,"ModelTexture"),0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,this->ModelTextureID);
    ShaderInstance->ActivateShader();

    glDrawElements(GL_TRIANGLES,this->Indices.size(),GL_UNSIGNED_INT,0);
    this->ModelVertexArray.Unbind();
}

void Model::DestroyModel(void)
{this->ModelVertexArray.Delete();}