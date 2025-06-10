#ifndef __VERTEX_ARRAY_HEADER__
#define __VERTEX_ARRAY_HEADER__

#include <vector>
#include "../Dependancies/glad/glad.h"
#include "VertexBuffer.h"

class VertexArrayObject
{
    public:
                        VertexArrayObject(void);
        void            CreateVertexArray(void);
        void            Bind(void);
        void            Unbind(void);
        void            Delete(void);
        void            LinkAttribute(VertexBufferObject& VertexBuffer,GLuint Layout,GLuint Components,GLenum Type,GLsizeiptr Stride,void* Offset);
        GLuint          VertexArray;
};

#endif