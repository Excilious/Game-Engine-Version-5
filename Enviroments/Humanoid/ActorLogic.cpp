#include "ActorLogic.h"
#include "../Common/Console.h"

ActorLogic::ActorLogic(void) {}

glm::vec3 ActorLogic::UpdateRayCollision(BinarySpaceCollisions* SceneCollider,const glm::vec3 Start,const glm::vec3 End)
{
    glm::vec3 NextPosition = SceneCollider->TraceRay(Start,End);
    if (NextPosition == Start)
    {
        SysPrint("Recursing again...",2);
        this->UpdateRayCollision(SceneCollider,glm::vec3(Start.x,Start.y+2.0f,Start.z),glm::vec3(End.x,End.y+2.0f,End.z));
    }
    return NextPosition;
}

void ActorLogic::UpdateActorCollider(BinarySpaceCollisions* SceneCollider,glm::vec3 MinimumBounds,glm::vec3 MaximumBounds,glm::vec3 NextPosition,float DeltaTime)
{
    glm::vec3 FinalPosition = glm::vec3(0.0f,0.0f,0.0f);
    if (!std::get<bool>(GetFromRegister("noclip")))
    {
        if (!SceneCollider->Grounded)   {this->GravityVelocity -= std::get<float>(GetFromRegister("mass"))*std::get<float>(GetFromRegister("gravity"));}
        else                            {this->GravityVelocity = 0;}

        NextPosition.y += this->GravityVelocity*DeltaTime;
        if (this->ModelPosition != NextPosition)
        {
            MinimumBounds.y -= 15;
            MaximumBounds.y -= 15;
            this->NewPosition = SceneCollider->TraceBox(
                this->ModelPosition,NextPosition,
                MinimumBounds,MaximumBounds
            );   
        
            if (this->NewPosition != NextPosition)
            {
                this->StepFrom  = this->ModelPosition+glm::vec3(0.0f,8.0f,0.0f);
                this->StepTo    = NextPosition+glm::vec3(0.0f,8.0f,0.0f);

                this->NewPosition = SceneCollider->TraceBox(
                   this->ModelPosition,NextPosition,
                   MinimumBounds,MaximumBounds
                );

                if (this->NewPosition == this->StepTo)
                {FinalPosition = this->NewPosition;}
            }
        }
    }
    else
    {this->NewPosition = NextPosition;}
    FinalPosition = this->NewPosition;
    this->ModelPosition = FinalPosition;
}