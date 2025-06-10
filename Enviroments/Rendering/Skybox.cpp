#include "Skybox.h"
#include "../Common/Console.h"

std::vector<float> SkyboxVertices =
{
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f
};

std::vector<unsigned int> SkyboxIndices =
{
	1, 2, 6,
	6, 5, 1,
	0, 4, 7,
	7, 3, 0,
	4, 5, 6,
	6, 7, 4,
	0, 3, 2,
	2, 1, 0,
	0, 1, 5,
	5, 4, 0,
	3, 7, 6,
	6, 2, 3
};

Skybox::Skybox(void) {}

void Skybox::InitiliseSkybox(int WindowWidth,int WindowHeight,float SkyNear,float SkyFar,Parser *InstanceParser,float FieldOfView)
{
    this->Width         = WindowWidth;
    this->Height        = WindowHeight;
    this->Near          = SkyNear;
    this->Far           = SkyFar;
    this->FieldOfView   = FieldOfView;

    this->VertexArray.CreateVertexArray();
    this->VertexArray.Bind();
    stbi_set_flip_vertically_on_load(false);
    VertexBufferObject  VertexBuffer(SkyboxVertices);
    ElementBufferObject ElementBuffer(SkyboxIndices);
    this->VertexArray.LinkAttribute(VertexBuffer,0,3,GL_FLOAT,3*sizeof(float),(void*)0);

    VertexBuffer.Unbind();
    this->VertexArray.Unbind();
    ElementBuffer.Unbind();

    glGenTextures(1,&this->CubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP,this->CubemapTexture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
    

    this->GenerateImageBuffers(InstanceParser);
    for (unsigned int Index = 0; Index < this->ImageBuffers.size(); Index++)
    {
        if (this->ImageBuffers[Index])
        {glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+Index,0,GL_RGB,this->ImageDimensions,this->ImageDimensions,0,GL_RGB,GL_UNSIGNED_BYTE,this->ImageBuffers[Index]);}
    }
    for (int Index = 0; Index < this->ImageBuffers.size(); Index++)
    {if (this->ImageBuffers[Index]) {delete this->ImageBuffers[Index];};}
}

void Skybox::GenerateImageBuffers(Parser *InstanceParser)
{
    this->ImageBuffers.push_back(InstanceParser->GetGlobalImageBuffer("Right.imgbuf"));
    this->ImageBuffers.push_back(InstanceParser->GetGlobalImageBuffer("Left.imgbuf"));
    this->ImageBuffers.push_back(InstanceParser->GetGlobalImageBuffer("Top.imgbuf"));
    this->ImageBuffers.push_back(InstanceParser->GetGlobalImageBuffer("Bottom.imgbuf"));
    this->ImageBuffers.push_back(InstanceParser->GetGlobalImageBuffer("Front.imgbuf"));
    this->ImageBuffers.push_back(InstanceParser->GetGlobalImageBuffer("Back.imgbuf"));
}

void Skybox::RenderSkybox(glm::vec3 Position,glm::vec3 Orientation,glm::vec3 UpPosition,Shader* SkyboxShader)
{
    glDepthFunc(GL_LEQUAL);
    SkyboxShader->ActivateShader();
    SkyboxShader->SetMatrix4(glm::mat4(glm::mat3(glm::lookAt(Position,Position+Orientation,UpPosition))),"View");
    SkyboxShader->SetMatrix4(glm::perspective(glm::radians(this->FieldOfView),(float)this->Width/this->Height,this->Near,this->Far),"Projection");

    this->VertexArray.Bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP,this->CubemapTexture);
    glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,0);
    this->VertexArray.Unbind();
    glDepthFunc(GL_LESS);
}

void Skybox::Destroy(void)
{for (int Index = 0; Index < this->ImageBuffers.size(); Index++) {if (this->ImageBuffers[Index]) {stbi_image_free(this->ImageBuffers[Index]);}}}