#ifndef __MODEL_HEADER__
#define __MODEL_HEADER__

#include "Mesh.h"
#include "Buffers/VertexArray.h"
#include "Buffers/ElementBuffer.h"
#include "Camera.h"
#include "../Common/Shader.h"

class Model: public Mesh
{
    public:
                                        Model(void);
        void                            DrawModel(Shader* ShaderInstance,Camera* CameraInstance,float NearPlane,float FarPlane,float FieldOfView,glm::mat4 CameraMatrix);
        void                            SetModelTexture(glm::vec2 TextureSize = glm::vec2(1024,1024));
        void                            UpdateMatrix(glm::vec3 Position = glm::vec3(0.0f,0.0f,0.0f),glm::quat Rotation = glm::quat(0.0f,0.0f,0.0f,0.0f),glm::vec3 Scale = glm::vec3(0.0f,0.0f,0.0f));
        void                            LoadFromGLTF(Parser* EngineParser,std::string SceneSource,glm::vec3 Translation,glm::quat Rotation,glm::vec3 Scale,std::string ImageTexture,glm::vec2 TextureSize);
        void                            DestroyModel(void);
        std::vector<float>              Vertices;
        std::vector<unsigned int>       Indices;
        unsigned char*                  ModelTextureData;
        unsigned int                    ModelTextureID;

        glm::vec3                       ModelPosition;
        glm::vec3                       ModelScale;
        glm::quat                       ModelRotation;
        glm::mat4                       TranslationMatrix;
    private:
        glm::mat4                       RotationMatrix;
        glm::mat4                       ScaleMatrix;
        VertexArrayObject               ModelVertexArray;
};     

#endif