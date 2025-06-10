#ifndef __LIGHTING_HEADER__
#define __LIGHTING_HEADER__

#include <vector>
#include "../../Enviroments/Common/Shader.h"

typedef struct
{
    glm::vec3   Colour;
    glm::vec3   Position;
    glm::vec3   Orientation;
    float       Brightness;
} LightBlock;

class Lighting
{
    public:
                                    Lighting(void);
        void                        CreateLight(glm::vec3 Colour,glm::vec3 Position,float Brightness,glm::vec3 Orientation = glm::vec3(0.0f,-1.0f,0.0f));
        void                        ResolveModelLighting(Shader* ModelShader,glm::vec3 CameraPosition,float AmbienceStrength,int SpecularStrength);
        void                        ResolveBinarySpaceLighting(Shader* ModelShader,std::vector<glm::vec3> LightingPosition,glm::vec3 CameraPosition);
        std::vector<LightBlock>     WorldLights;
};

#endif