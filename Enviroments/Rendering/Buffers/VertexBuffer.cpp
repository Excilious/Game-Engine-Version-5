#include "VertexBuffer.h"

VertexBufferObject::VertexBufferObject(std::vector<float>& Vertices)
{
    glGenBuffers(1,&this->VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,this->VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,Vertices.size()*sizeof(float),Vertices.data(),GL_STATIC_DRAW);
}

VertexBufferObject::VertexBufferObject(std::vector<glm::vec3>& Vertices)
{
    glGenBuffers(1,&this->VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,this->VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,Vertices.size()*sizeof(glm::vec3),Vertices.data(),GL_STATIC_DRAW);
}

VertexBufferObject::VertexBufferObject(std::vector<SceneVertex>& Vertices)
{
    glGenBuffers(1,&this->VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,this->VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,Vertices.size()*sizeof(SceneVertex),Vertices.data(),GL_STATIC_DRAW);
}

void VertexBufferObject::Bind(void)     {glBindBuffer(GL_ARRAY_BUFFER,this->VertexBuffer);}
void VertexBufferObject::Unbind(void)   {glBindBuffer(GL_ARRAY_BUFFER,0);}
void VertexBufferObject::Delete(void)   {glDeleteBuffers(1,&this->VertexBuffer);}