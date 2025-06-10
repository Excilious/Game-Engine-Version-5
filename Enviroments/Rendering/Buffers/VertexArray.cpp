#include "VertexArray.h"

VertexArrayObject::VertexArrayObject(void)      {}

void VertexArrayObject::CreateVertexArray(void) {glGenVertexArrays(1,&this->VertexArray);}
void VertexArrayObject::Bind(void)              {glBindVertexArray(this->VertexArray);}
void VertexArrayObject::Unbind(void)            {glBindVertexArray(0);}
void VertexArrayObject::Delete(void)            {glDeleteVertexArrays(1,&this->VertexArray);}

void VertexArrayObject::LinkAttribute(VertexBufferObject& VertexBuffer,GLuint Layout,GLuint Components,GLenum Type,GLsizeiptr Stride,void* Offset)
{
    VertexBuffer.Bind();
    glVertexAttribPointer(Layout,Components,Type,GL_FALSE,Stride,Offset);
    glEnableVertexAttribArray(Layout);
    VertexBuffer.Unbind();
}