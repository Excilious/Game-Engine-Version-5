#ifndef __ENTITY_HEADER__
#define __ENTITY_HEADER__

#include "../Rendering/Model.h"

typedef enum
{
    ENT_STATIC,
    ENT_DYNAMIC
} EntityType;

class Entity : public Model
{   
    public:
                        Entity(void);
        void            CreateEntity(const std::string EntityName,const std::string ModelFile,Parser* NewParser,const glm::vec3 Position,const glm::quat Rotation,const glm::vec3 Scale,const std::string TextureReference,const glm::vec2 TextureSize);
        void            CreateShadowMap(void);
        float           ModelAmbience;
        float           ModelSpecular;
    private:
        unsigned int    ShadowMapFrameBuffer;
        unsigned int    ShadowMapWidth,ShadowMapHeight = 2048;
        unsigned int    ShadowMap;
        float           ClampColour[4] = {1.0f,1.0f,1.0f,1.0f};

};

#endif