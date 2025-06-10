#include "HumanoidPhysics.h"
#include "../Utilites/BroadPhaseLayerInterface.h"

HumanoidPhysics::HumanoidPhysics()
{}

void HumanoidPhysics::CreateHumanoidBoundCapsule(JPH::PhysicsSystem *EnginePhysicsSystem)
{
    CharacterStandingShape = JPH::RotatedTranslatedShapeSettings(
        JPH::Vec3(0, 0.5f * 1.35f + 0.3f, 0), 
        JPH::Quat::sIdentity(), 
        new JPH::CapsuleShape(0.5f * 1.35f, 0.3f)
    ).Create().Get();

    JPH::Ref<JPH::CharacterSettings> NewCharacterSettings = new JPH::CharacterSettings();
    NewCharacterSettings->mMaxSlopeAngle    = JPH::DegreesToRadians(45.0f);
    NewCharacterSettings->mLayer            = Layers::MOVING;
    NewCharacterSettings->mShape            = this->CharacterStandingShape;
    NewCharacterSettings->mFriction         = 0.5f;
    
    this->NewCharacter = new JPH::Character(NewCharacterSettings,JPH::Vec3(0.0f,0.0f,0.0f),JPH::Quat::sIdentity(),0,EnginePhysicsSystem);
    this->NewCharacter->AddToPhysicsSystem();
}

void HumanoidPhysics::UpdateHumanoidPosition(glm::vec3 Position)
{
    this->NewCharacter->SetPosition(JPH::Vec3Arg(Position.x,Position.y,Position.z));
}

void HumanoidPhysics::HandleHumanoidPhysicsMovement(JPH::Vec3Arg MovementDirection,bool Jump,bool SwitchStance,float DeltaTime)
{
    JPH::Vec3 LocalMovementDirection;
    JPH::Character::EGroundState HumanoidGroundState = this->NewCharacter->GetGroundState();
    if (HumanoidGroundState == JPH::Character::EGroundState::OnSteepGround || HumanoidGroundState == JPH::Character::EGroundState::NotSupported)
    {
        JPH::Vec3 Normal = this->NewCharacter->GetGroundNormal();
        Normal.SetY(0.0f);
        float Dot = Normal.Dot(LocalMovementDirection);
        if (Dot < 0.0f) {LocalMovementDirection -= (Dot * Normal) / Normal.LengthSq();}
    }

    if (this->NewCharacter->IsSupported())
    {
        JPH::Vec3 CurrentVelocity = this->NewCharacter->GetLinearVelocity();
        JPH::Vec3 DesiredVelocity = 0.01 * LocalMovementDirection;
    
        if (!DesiredVelocity.IsNearZero() || CurrentVelocity.GetY() < 0.0f || !this->NewCharacter->IsSupported())
        {DesiredVelocity.SetY(CurrentVelocity.GetY());}
        JPH::Vec3 NewVelocity = 0.75f * CurrentVelocity + 0.25f * DesiredVelocity;

        if (Jump && HumanoidGroundState == JPH::Character::EGroundState::OnGround)
        {NewVelocity += JPH::Vec3(0,0.02,0);}
        this->NewCharacter->SetLinearVelocity(NewVelocity);
    }
}