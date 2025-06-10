#ifndef __ELEMENT_BUFFER_HEADER__
#define __ELEMENT_BUFFER_HEADER__

#include <vector>
#include "../Dependancies/glad/glad.h"
#include "../Dependancies/GLFW/glfw3.h"
#include "../Dependancies/glm/glm.hpp"

class ElementBufferObject
{
    public:
                    ElementBufferObject(std::vector<GLuint>& Indices);
        void        Bind(void);
        void        Unbind(void);
        void        Delete(void);
        GLuint      ElementBuffer;
};

#endif