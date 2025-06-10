#ifndef __HUMANOID_PHYSICS_HEADER__
#define __HUMANOID_PHYSICS_HEADER__

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Character/Character.h>
#include <Jolt/Physics/Character/CharacterVirtual.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h>
#include "../../../Dependancies/glm/glm.hpp"

class HumanoidPhysics
{
    public:
        HumanoidPhysics();
        void CreateHumanoidBoundCapsule(JPH::PhysicsSystem *EnginePhysicsSystem);
        void HandleHumanoidPhysicsMovement(JPH::Vec3Arg MovementDirection,bool Jump,bool SwitchStance,float DeltaTime);
        void UpdateHumanoidPosition(glm::vec3 Position);
    private:
        JPH::Ref<JPH::Character>    NewCharacter;
        JPH::RefConst<JPH::Shape>   CharacterStandingShape;
};

#endif