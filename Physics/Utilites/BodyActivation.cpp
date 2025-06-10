#include "BodyActivation.h"
#include "../../Common/Console.h"

void MyBodyActivationListener::OnBodyActivated(const JPH::BodyID &inBodyID, JPH::uint64 inBodyUserData)
{
    SysPrint("Body activated",1);
}

void MyBodyActivationListener::OnBodyDeactivated(const JPH::BodyID &inBodyID, JPH::uint64 inBodyUserData)
{
    SysPrint("Body slept",1);
}