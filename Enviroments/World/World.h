#ifndef __WORLD_HEADER__
#define __WORLD_HEADER__

#include <map>
#include "Instance.h"
#include "../Enviroments/Rendering/Scene/Scene.h"
#include "../Rendering/Terrain/Water.h"
#include "../Rendering/Terrain/Terrain.h"
#include "../Rendering/Camera.h"
#include "../Modules/Viewport.h"

class World
{
    public:
                                                    World(void);
        void                                        GenerateWorld(GLFWwindow* EngineWindow,Parser* WorldParser,float Near,float Far);
        void                                        RenderWorld(GLFWwindow* EngineWindow,Player* EnginePlayer,Camera *EngineCamera,float Near,float Far,float FieldOfView,unsigned int ReflectanceTexture,float DeltaTime,float PlayerSpeed);
        void                                        DestroyWorld(void);
        int                                         LevelIndex;
        Viewport                                    WeaponViewport;
        Instance                                    NewInstance;
        
        glm::mat4                                   TranslationMatrix;
        glm::mat4                                   RotationMatrix;
        glm::mat4                                   CollisionRotationMatrix;
        glm::mat4                                   ScaleMatrix;
    private:
        std::vector<float>                          GetSceneVertices(Parser* CurrentParser);
        std::vector<unsigned int>                   GetSceneIndices(Parser* CurrentParser);
        std::map<int,std::vector<float>>            VertexCache;
        std::map<int,std::vector<unsigned int>>     IndicesCache;

        glm::mat4                                   PlayerTranslation;
        glm::mat4                                   PlayerRotation;
        glm::mat4                                   PlayerScale;
        nlohmann::json                              NewJson;
};

#endif