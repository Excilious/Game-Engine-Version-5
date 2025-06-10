#ifndef __INSTANCE_HEADER__
#define __INSTANCE_HEADER__

#include <vector>
#include "../Common/Parser.h"
#include "../Humanoid/Actor.h"
#include "../Rendering/Camera.h"
#include "../Rendering/Lighting.h"
#include "../../Enviroments/Dynamic/Entity.h"

typedef struct
{
    Entity           NewEntityInstance;
    EntityType       NewEntityType;
    Shader           ShaderEntity;
} EntityNode;

class Instance
{
    public:
                                    Instance(void);
        void                        GenerateStaticEntity(Parser *EntityParser,std::string ModelName,std::string EntityName,std::string ShaderVert,std::string ShaderFrag,glm::vec3 Position,glm::vec3 Scale,glm::quat Rotation,std::string TextureReference,glm::vec2 TextureSize);
        void                        RenderEntities(Camera *EngineCamera,float Near,float Far,float FieldOfView,glm::mat4 CameraMatrix);
        void                        DestroyAll(void);
        Lighting                    WorldEntityLighting;
        std::vector<EntityNode>     EntityList;
};

#endif