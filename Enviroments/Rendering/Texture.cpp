#define STB_IMAGE_IMPLEMENTATION
#include "../Common/Console.h"
#include "Texture.h"

Texture::Texture(void): Type("diffuse") {}

unsigned int Texture::GetTextureFromFile(std::string Path,bool Gamma)
{
    unsigned int TextureID;
    glGenTextures(1,&TextureID);
    
    int Width,Height,Components;
    unsigned char *Data = stbi_load(Path.c_str(),&Width,&Height,&Components,0);
    if (Data)
    {
        GLenum Format;
        if (Components == 1)    {Format = GL_RED;}
        if (Components == 3)    {Format = GL_RGB;}
        if (Components == 4)    {Format = GL_RGBA;}

        glBindTexture(GL_TEXTURE_2D,TextureID);
        glTexImage2D(GL_TEXTURE_2D,0,Format,Width,Height,0,Format,GL_UNSIGNED_BYTE,Data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    }
    stbi_image_free(Data);
    return TextureID;
}

void Texture::CreateBufferTexture(unsigned char* BufferData,unsigned int Width,unsigned int Height)
{
    SysPrint("Creating buffer texture of width: "+std::to_string(Width)+std::to_string(Height),1);
    glGenTextures(1,&this->TextureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,this->TextureID);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,Width,Height,0,GL_RGBA,GL_UNSIGNED_BYTE,BufferData);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,0);
    free(BufferData);
}

void Texture::CreateNormalTexture(const char* Image,std::string TextureType)
{
    int WidthImage,HeightImage,ColourNumberChannel;
    unsigned char* Bytes = stbi_load(Image,&WidthImage,&HeightImage,&ColourNumberChannel,0);
    stbi_set_flip_vertically_on_load(true);
    this->Type = TextureType;

    glGenTextures(1,&this->TextureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,this->TextureID);
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    if (ColourNumberChannel == 4) {glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,WidthImage,HeightImage,0,GL_RGBA,GL_UNSIGNED_BYTE,Bytes);} 
    else if (ColourNumberChannel == 3) {glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,WidthImage,HeightImage,0,GL_RGB,GL_UNSIGNED_BYTE,Bytes);} 
    else if (ColourNumberChannel == 1) {glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,WidthImage,HeightImage,0,GL_RED,GL_UNSIGNED_BYTE,Bytes);} 
    else {SysPrint("Cannot find texture type.",2);}

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(Bytes);
    glBindTexture(GL_TEXTURE_2D,0);
}

void Texture::TextureUnit(Shader* NewShader,const char* Uniform,GLuint Unit)
{
    GLuint TextureUnit = glGetUniformLocation(NewShader->Program,Uniform);
    NewShader->ActivateShader();
    glUniform1i(TextureUnit,Unit);
}

void Texture::Bind(void)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,this->TextureID);
}

void Texture::Unbind(void) {glBindTexture(GL_TEXTURE_2D,0);}
void Texture::Delete(void) {glDeleteTextures(1,&this->TextureID);}

