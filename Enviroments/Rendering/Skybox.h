#ifndef __SKYBOX_HEADER__
#define __SKYBOX_HEADER__
#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <string>
#include <vector>

#include "../Dependancies/glm/glm.hpp"
#include "../Dependancies/glm/gtc/matrix_transform.hpp"
#include "../Dependancies/glm/gtc/type_ptr.hpp"
#include "../Dependancies/glm/gtx/rotate_vector.hpp"
#include "../Dependancies/glm/gtx/vector_angle.hpp"

#include "../Dependancies/glad/glad.h"
#include "../Dependancies/GLFW/glfw3.h"

#include "Buffers/ElementBuffer.h"
#include "Buffers/VertexArray.h"
#include "Buffers/VertexBuffer.h"
#include "../Common/Parser.h"
#include "Texture.h"

class Skybox
{
    public:
                                        Skybox(void);
        void                            InitiliseSkybox(int WindowWidth,int WindowHeight,float SkyNear,float SkyFar,Parser *InstanceParser,float FieldOfView);
        void                            RenderSkybox(glm::vec3 Position,glm::vec3 Orientation,glm::vec3 UpPosition,Shader* SkyboxShader);
        void                            Destroy(void);

        unsigned int                    SkyboxVertexArrayObject;
        unsigned int                    SkyboxVertexBufferObject;
        unsigned int                    SkyboxVertexElementObject;
        unsigned int                    CubemapTexture;
    private:
        std::vector<unsigned char*>     ImageBuffers;
        VertexArrayObject               VertexArray;
        int                             ImageDimensions = 1024;
        int                             Width;
        int                             Height;
        float                           Near;
        float                           Far;
        float                           FieldOfView;
        void                            GenerateImageBuffers(Parser *InstanceParser);
};

#endif