#include "ElementBuffer.h"

ElementBufferObject::ElementBufferObject(std::vector<GLuint>& Indices)
{
    glGenBuffers(1,&this->ElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->ElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,Indices.size()*sizeof(GLuint),Indices.data(),GL_STATIC_DRAW);
}

void ElementBufferObject::Bind(void)    {glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->ElementBuffer);}
void ElementBufferObject::Unbind(void)  {glBindBuffer(GL_ARRAY_BUFFER,0);}
void ElementBufferObject::Delete(void)  {glDeleteBuffers(1,&this->ElementBuffer);}

