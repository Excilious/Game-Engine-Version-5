#include "Scene.h"

Scene::Scene(void) {}

void Scene::LoadFromGLTF(Parser* EngineParser,std::string SceneSource,glm::mat4 Translation,glm::mat4 Rotation,glm::mat4 Scale,std::vector<LightBlock> LightBlockList,float Near,float Far)
{
    this->SceneVertexArray.CreateVertexArray();
    this->LoadPartitionUsingGLTF(EngineParser,SceneSource);
    if (this->PartitionVertices.size() != 0 && this->PartitionIndices.size() != 0)
    {
        this->SceneTextureData      = EngineParser->GetGlobalImageBuffer("Top.imgbuf");
        if (this->SceneTextureData == (unsigned char*)"") {SysPrint("Failed to load scene textures!",3); return;}

        this->TranslationMatrix     = Translation;
        this->RotationMatrix        = Rotation;
        this->ScaleMatrix           = Scale;
        this->Vertices              = this->FloatPositionVectors;
        this->Indices               = PartitionIndices;
        this->SetSceneTexture();

        #if FUSION_USE_FRAMEBUFFER
            this->GenerateLevelShadowMap(glm::vec3(0.0f,0.0f,0.0f),Near,Far);
        #endif

        this->SceneVertexArray.Bind();
        VertexBufferObject SceneVertexBuffer(this->PartitionVertices);
        ElementBufferObject SceneElementBuffer(this->PartitionIndices);

        this->SceneVertexArray.LinkAttribute(SceneVertexBuffer,0,3,GL_FLOAT,sizeof(SceneVertex),(void*)0);
        this->SceneVertexArray.LinkAttribute(SceneVertexBuffer,1,3,GL_FLOAT,sizeof(SceneVertex),(void*)(3*sizeof(float)));
        this->SceneVertexArray.LinkAttribute(SceneVertexBuffer,2,3,GL_FLOAT,sizeof(SceneVertex),(void*)(6*sizeof(float)));
        this->SceneVertexArray.LinkAttribute(SceneVertexBuffer,3,2,GL_FLOAT,sizeof(SceneVertex),(void*)(9*sizeof(float)));

        this->SceneVertexArray.Unbind();
        SceneVertexBuffer.Unbind();
        SceneElementBuffer.Unbind();
    }
    else
    {SysPrint("Failed to load "+SceneSource+" level parition.",2);}
}

void Scene::GenerateLevelShadowMap(glm::vec3 LightPosition,float NearPlane,float FarPlane)
{
    this->ShadowMapWidth            = 2048;
    this->ShadowMapHeight           = 2048;
    this->ShadowMapClampColour[0]   = 1.0f;
    this->ShadowMapClampColour[1]   = 1.0f;
    this->ShadowMapClampColour[2]   = 1.0f;
    this->ShadowMapClampColour[3]   = 1.0f;

    glGenTextures(1, &this->ShadowMap);
    glBindTexture(GL_TEXTURE_2D, this->ShadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, ShadowMapWidth, ShadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT,NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, ShadowMapClampColour);

    glGenFramebuffers(1, &this->ShadowMapFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, this->ShadowMapFrameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->ShadowMap, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {SysPrint("Shadow Framebuffer is not complete!",3);}

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glm::mat4 OrientationProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, NearPlane, FarPlane);
    glm::mat4 LightView = glm::lookAt(LightPosition, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    this->LightProjection = OrientationProjection * LightView;
}

void Scene::SetSceneTexture()
{
    glGenTextures(1,&this->SceneTextureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,this->SceneTextureID);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,1024,1024,0,GL_RGB,GL_UNSIGNED_BYTE,this->SceneTextureData);
    glGenerateMipmap(GL_TEXTURE_2D);
    free(this->SceneTextureData);
    glBindTexture(GL_TEXTURE_2D,0);
}

void Scene::GenerateLevelParitionData(std::vector<float> Vertices,std::vector<unsigned int> Indices,glm::mat4 Translation,glm::mat4 Rotation,glm::mat4 Scale)
{
    this->Vertices              = Vertices;
    this->Indices               = Indices;
    this->TranslationMatrix     = Translation;
    this->RotationMatrix        = Rotation;
    this->ScaleMatrix           = Scale;

    this->SceneVertexArray.Bind();
    VertexBufferObject* SceneVertexBuffer   = new VertexBufferObject(this->Vertices);
    ElementBufferObject* SceneElementBuffer = new ElementBufferObject(this->Indices);
  
    this->SceneVertexArray.LinkAttribute(*SceneVertexBuffer,0,3,GL_FLOAT,6*sizeof(float),(void*)0);
    this->SceneVertexArray.LinkAttribute(*SceneVertexBuffer,1,3,GL_FLOAT,6*sizeof(float),(void*)(3*sizeof(float)));

    this->SceneVertexArray.Unbind();
    SceneVertexBuffer->Unbind();
    SceneElementBuffer->Unbind();
    if (SceneVertexBuffer)  {delete SceneVertexBuffer;}
    if (SceneElementBuffer) {delete SceneElementBuffer;}
}

void Scene::DrawScene(Shader* ShaderInstance,Camera* CameraInstance,float NearPlane,float FarPlane,float FieldOfView)
{
    #if FUSION_USE_FRAMEBUFFER
        glBindFramebuffer(GL_FRAMEBUFFER,this->ShadowMapFrameBuffer);
        glClear(GL_DEPTH_BUFFER_BIT);
    #endif

    ShaderInstance->SetMatrix4(this->LightProjection,"LightProjection");

    ShaderInstance->ActivateShader();
    this->SceneVertexArray.Bind();
    
    CameraInstance->UpdateMatrix(FieldOfView,NearPlane,FarPlane);
    CameraInstance->SetMatrix(ShaderInstance,"CameraMatrix");
    ShaderInstance->SetVector3(CameraInstance->Position,"CameraPosition");
    ShaderInstance->SetVector3(glm::vec3(0.0f,1.0f,0.0f),"Colour");
    ShaderInstance->SetMatrix4(this->TranslationMatrix,"Translation");
    ShaderInstance->SetMatrix4(this->RotationMatrix,"Rotation");
    ShaderInstance->SetMatrix4(this->ScaleMatrix,"Scale");
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,this->SceneTextureID);
    ShaderInstance->SetVector1(0,"SceneTexture");
    glActiveTexture(GL_TEXTURE0+1);
    glBindTexture(GL_TEXTURE_2D,this->ShadowMap);
    ShaderInstance->SetVector1(1,"ShadowMap");
    ShaderInstance->ActivateShader();

    glDrawElements(GL_TRIANGLES,this->Indices.size(),GL_UNSIGNED_INT,0);
    this->SceneVertexArray.Unbind();

    #if FUSION_USE_FRAMEBUFFER
        glBindFramebuffer(GL_FRAMEBUFFER,0);
    #endif
}

void Scene::DestroyScene()
{this->SceneVertexArray.Delete();}