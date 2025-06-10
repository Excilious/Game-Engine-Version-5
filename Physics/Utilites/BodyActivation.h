#ifndef __BODY_ACTIVATION_HEADER__
#define __BODY_ACTIVATION_HEADER__

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

class MyBodyActivationListener: public JPH::BodyActivationListener 
{
    public:
        virtual void OnBodyActivated(const JPH::BodyID &inBodyID, JPH::uint64 inBodyUserData) override;
        virtual void OnBodyDeactivated(const JPH::BodyID &inBodyID, JPH::uint64 inBodyUserData) override;
};

#endif