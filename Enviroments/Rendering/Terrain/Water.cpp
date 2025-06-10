#include "Water.h"
#include "../../Common/Console.h"

Water::Water(): ToggleFrameBuffer(false) {}

void Water::GenerateWaterVertices(Parser* EngineParser)
{
    this->WaterShader.CreateNewShader(EngineParser->GetGlobalDatapackData("Water.vert").c_str(),EngineParser->GetGlobalDatapackData("Water.frag").c_str(),true);
    this->WaterTexture.CreateNormalTexture("../../Textures/Water.png","diffuse");

    for (int X = 0; X < WATER_GRID_SIZE; X++)
    {
        for (int Z = 0; Z < WATER_GRID_SIZE; Z++)
        {
            float PositionX = X*WATER_GRID_SPACING;
            float PositionZ = Z*WATER_GRID_SPACING;
            float U         = (float)X/(WATER_GRID_SIZE-1);
            float V         = (float)Z/(WATER_GRID_SIZE-1);

            this->WaterVertices.insert(this->WaterVertices.end(),{PositionX,0.0f,PositionZ,0.0f,1.0f,0.0f,U,V});
        }
    }

    for (int X = 0; X < WATER_GRID_SIZE; X++)
    {
        for (int Z = 0; Z < WATER_GRID_SIZE; Z++)
        {
            unsigned int TopLeft         = X*WATER_GRID_SIZE+Z;
            unsigned int TopRight        = TopLeft+1;
            unsigned int BottomLeft      = TopLeft+WATER_GRID_SIZE;
            unsigned int BottomRight     = BottomLeft+1;
            
            this->WaterIndices.insert(this->WaterIndices.end(),{TopLeft,BottomLeft,TopRight});
            this->WaterIndices.insert(this->WaterIndices.end(),{TopRight,BottomLeft,BottomRight});
        }
    }
}

void Water::CreateReflectionFrameBuffer(void)
{
    glGenFramebuffers(1,&this->ReflectionFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER,this->ReflectionFrameBuffer);

    glGenTextures(1,&this->ReflectionTexture);
    glBindTexture(GL_TEXTURE_2D,this->ReflectionTexture);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,this->WaterWidth,this->WaterHeignt,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,this->ReflectionTexture,0);

    glGenRenderbuffers(1,&this->ReflectionDepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER,this->ReflectionDepthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,this->WaterWidth,this->WaterHeignt);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,this->ReflectionDepthBuffer);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {SysPrint("Reflection Framebuffer Is Not Complete Yet!",3);}
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void Water::GenerateWaterBuffers(void)
{
    glGenVertexArrays(1,&this->WaterVertexArray);
    glGenBuffers(1,&this->WaterVertexBuffer);
    glGenBuffers(1,&this->WaterElementBuffer);

    glBindVertexArray(this->WaterVertexArray);
    glBindBuffer(GL_ARRAY_BUFFER,this->WaterVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,this->WaterVertices.size()*sizeof(float),this->WaterVertices.data(),GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->WaterElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,this->WaterIndices.size()*sizeof(unsigned int),this->WaterIndices.data(),GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
}

void Water::RenderWater(Camera* EngineCamera,unsigned int SkyboxCubemapTexture)
{
    this->DeltaTimeValue = glfwGetTime();
    this->WaterShader.ActivateShader();
    glUniformMatrix4fv(glGetUniformLocation(this->WaterShader.Program,"projection"),1,GL_FALSE,glm::value_ptr(EngineCamera->ProjectionMatrix));
    glUniformMatrix4fv(glGetUniformLocation(this->WaterShader.Program,"view"),1,GL_FALSE,glm::value_ptr(EngineCamera->ViewMatrix));
    glUniform1f(glGetUniformLocation(this->WaterShader.Program,"time"),this->DeltaTimeValue);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,this->ReflectionTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP,SkyboxCubemapTexture);
    glUniform3f(glGetUniformLocation(this->WaterShader.Program,"camerapos"),EngineCamera->Position.x,EngineCamera->Position.y,EngineCamera->Position.z);
    glUniform1i(glGetUniformLocation(this->WaterShader.Program,"skyboxreflection"), 0);
    this->WaterTexture.TextureUnit(&this->WaterShader,"waterTexture",1);
    this->WaterTexture.Bind();

    glBindVertexArray(this->WaterVertexArray);
    glDrawElements(GL_TRIANGLES,(WATER_GRID_SIZE-1)*(WATER_GRID_SIZE-1)*6,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);
}

void Water::Destroy(void)
{
    glDeleteBuffers(1,&this->WaterVertexArray);
    this->WaterShader.DestroyShader();
    this->WaterTexture.Delete();
}