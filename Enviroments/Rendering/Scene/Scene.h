#ifndef __SCENE_HEADER__
#define __SCENE_HEADER__

#include "Partition.h"
#include "../../Common/Shader.h"
#include "../Buffers/VertexArray.h"
#include "../Buffers/ElementBuffer.h"
#include "../Camera.h"
#include "../Lighting.h"

#ifndef FUSION_USE_FRAMEBUFFER
    #define FUSION_USE_FRAMEBUFFER 0
#endif

class Scene: public Partition
{
    public:
                                    Scene(void);
        void                        SetSceneTexture(void);
        void                        GenerateLevelParitionData(std::vector<float> Vertices,std::vector<unsigned int> Indices,glm::mat4 Translation,glm::mat4 Rotation,glm::mat4 Scale);
        void                        GenerateLevelShadowMap(glm::vec3 LightPosition,float NearPlane,float FarPlane);
        void                        DrawScene(Shader* ShaderInstance,Camera* CameraInstance,float NearPlane,float FarPlane,float FieldOfView);
        void                        LoadFromGLTF(Parser* EngineParser,std::string SceneSource,glm::mat4 Translation,glm::mat4 Rotation,glm::mat4 Scale,std::vector<LightBlock> LightBlockList,float Near,float Far);
        void                        DestroyScene(void);
        std::vector<float>          Vertices;
        std::vector<unsigned int>   Indices;
    private:
        glm::mat4                   LightProjection;
        glm::mat4                   TranslationMatrix;
        glm::mat4                   RotationMatrix;
        glm::mat4                   ScaleMatrix;
        unsigned char*              SceneTextureData;
        unsigned int                SceneTextureID;
        unsigned int                ShadowMapFrameBuffer;
        unsigned int                ShadowMapWidth,ShadowMapHeight = 2048;
        unsigned int                ShadowMap;
        float                       ShadowMapClampColour[4] = {1.0f,1.0f,1.0f,1.0f};
        VertexArrayObject           SceneVertexArray;
};     

#endif