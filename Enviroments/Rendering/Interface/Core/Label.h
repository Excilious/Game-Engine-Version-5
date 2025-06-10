#ifndef __LABEL_HEADER__
#define __LABEL_HEADER__
#define GLT_IMPLEMENTATION

#include <string>
#include "../Dependancies/glad/glad.h"
#include "../Dependancies/glm/glm.hpp"
#include "../Dependancies/GLFW/glfw3.h"
#include "../Dependancies/gltext/gltext.h"

class Label
{
    public:
                        Label(void);
        void            CreateLabel(std::string Text,glm::vec2 TextPosition,float TextScale,glm::vec3 TextColour);
        void            SetNewText(std::string Text);
        void            RenderText(void);
        void            DestroyText(void);
        std::string     TextContent;
        glm::vec2       TextPosition;
        glm::vec3       TextColour;
        float           Scale;
    private:
        GLTtext         *TextInstance;
        unsigned int    TextVertexArray;
        unsigned int    TextVertexBuffer;
};

#endif