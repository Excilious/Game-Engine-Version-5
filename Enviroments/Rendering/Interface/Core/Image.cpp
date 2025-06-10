#include "Image.h"

Image::Image(void) {}

void Image::CreateImage(unsigned char* ImageBuffer,glm::vec2 Position,glm::vec2 Scale,int Width,int Height,float Rotation,std::string FrameVertex,std::string FrameFragment,int ImageWidth,int ImageHeight)
{
    this->CreateFrame(Position,Scale,Width,Height,glm::vec3(0.0f,0.0f,0.0f),Rotation,FrameVertex,FrameFragment);
    this->OverrideFrameRendering = true;
    this->NewTextureDiffuse.CreateBufferTexture(ImageBuffer);
    this->NewTextureDiffuse.TextureUnit(&this->NewShader,"IsImage",0);
    this->NewTextureDiffuse.Bind();
    this->NewShader.ActivateShader();
}