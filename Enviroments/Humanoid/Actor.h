#ifndef __ACTOR_HEADER__
#define __ACTOR_HEADER__

#include "../Dynamic/Entity.h"
#include "Animation.h"

typedef enum 
{
    OWNER_PLAYER,
    OWNER_ENTITY,
    OWNER_NULL
} EntityOwnership;

struct BoundingBoxActor
{
    glm::vec3 Minimum;
    glm::vec3 Maximum;
};

class Actor: public Entity, public Animation
{
    public:
                            Actor(void);
        void                CreateActor(const std::string ActorName,const std::string ModelFile,Parser* NewParser,const glm::vec3 Position,const glm::quat Rotation,const glm::vec3 Scale,const std::string TextureReference,const glm::vec2 TextureSize);
        BoundingBoxActor    GetMinimumMaximum(void);
        EntityOwnership     CurrentOwnership;
};

#endif