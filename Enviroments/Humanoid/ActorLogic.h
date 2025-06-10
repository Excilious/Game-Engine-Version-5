#ifndef __ACTOR_LOGIC_HEADER__
#define __ACTOR_LOGIC_HEADER__

#include "Actor.h"
#include "../Modules/BinarySpaceCollisions.h"

class ActorLogic: public Actor
{
    public:
                        ActorLogic(void);
        glm::vec3       UpdateRayCollision(BinarySpaceCollisions* SceneCollider,const glm::vec3 Start,const glm::vec3 End);
        void            UpdateActorCollider(BinarySpaceCollisions* SceneCollider,const glm::vec3 MinimumBounds,glm::vec3 MaximumBounds,glm::vec3 NextPosition,float DeltaTime);
    private:
        glm::vec3       StepTo;
        glm::vec3       StepFrom;
        glm::vec3       NewPosition;
        float           GravityVelocity;
};

#endif