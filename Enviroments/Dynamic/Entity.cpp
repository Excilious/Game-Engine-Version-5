#include "Entity.h"

Entity::Entity(void): ModelAmbience(0.1f),ModelSpecular(0.5f) {}

void Entity::CreateShadowMap(void)
{
    glGenFramebuffers(1,&this->ShadowMapFrameBuffer);
    glGenTextures(1,&this->ShadowMap);
    glBindTexture(GL_TEXTURE_2D,this->ShadowMap);
    glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,this->ShadowMapWidth,this->ShadowMapHeight,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,this->ClampColour);
    glBindFramebuffer(GL_FRAMEBUFFER,this->ShadowMapFrameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,this->ShadowMap,0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void Entity::CreateEntity(const std::string EntityName,const std::string ModelFile,Parser* NewParser,const glm::vec3 Position,const glm::quat Rotation,const glm::vec3 Scale,const std::string TextureReference,const glm::vec2 TextureSize)
{
    this->LoadFromGLTF(NewParser,ModelFile,Position,Rotation,Scale,TextureReference,TextureSize);
}

