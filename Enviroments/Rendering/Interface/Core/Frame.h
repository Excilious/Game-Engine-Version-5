#ifndef __INTERFACE_H__
#define __INTERFACE_H__
#define GLT_IMPLEMENTATION

#include <vector>
#include <map>
#include <string>

#include "../Dependancies/glad/glad.h"
#include "../Dependancies/glm/glm.hpp"
#include "../Dependancies/GLFW/glfw3.h"
#include "../Dependancies/gltext/gltext.h"

#include "../Dependancies/glm/gtc/matrix_transform.hpp"
#include "../Dependancies/glm/gtc/type_ptr.hpp"

#include "../../../Common/Shader.h"
#include "../../Buffers/VertexArray.h"

class Frame
{
    public:
                            Frame(void);
        void                CreateFrame(glm::vec2 Position,glm::vec2 Scale,int Width,int Height,glm::vec3 Colour,float Rotation,std::string FrameVertex,std::string FrameFragment);
        void                AddTransparancy(float Transparency);
        void                AddCorners(float Radius);
        void                RenderInstance(void);
        void                DestroyInstance(void);

        float               WindowWidth;
        float               WindowHeight;
        float               Rotation;
        float               Transparency;
        float               TransparencyActive;
        float               Radius;
        bool                Active;
        bool                CornerActive;
        bool                OverrideFrameRendering;
        
        glm::vec2           Position;
        glm::vec2           Scale;
        glm::vec3           Colour;
        glm::mat4           OrthoMatrix;
        Shader              NewShader;

    private:    
        unsigned int        FrameVertexBuffer;
        glm::vec3           NormaliseColourValues(glm::vec3 ColourValue);
        glm::mat4           TranslationMatrix;
        glm::mat4           RotationMatrix;
        glm::mat4           ScaleMatrix;
        glm::mat4           ProjectionMatrix;
        glm::mat4           ModelMatrix;
        VertexArrayObject   FrameVertexArray;
};

#endif