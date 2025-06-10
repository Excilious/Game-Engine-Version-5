#ifndef __PHYSICS_HEADER__
#define __PHYSICS_HEADER__

#include <Jolt/Jolt.h>

#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>

#include <thread>
#include "Utilites/BodyActivation.h"
#include "Utilites/BroadPhaseLayerInterface.h"
#include "Utilites/ContactListener.h"
#include "Utilites/ObjectLayerPairFilter.h"

#include "../Common/Console.h"
#include "../Rendering/Camera.h"
#include "Actions/MeshShapeTransformation.h"

JPH_SUPPRESS_WARNINGS

class Physics
{
    public:
        Physics();
        void StartPhysicsEngine(std::vector<float> Vertices,std::vector<unsigned int> Indices);
        void RenderPhysicsEngine(Camera* EngineCamera);
        void CreateCharacterPhysics();
        void DestroyPhysicsEngine();
    private:
        //HumanoidPhysics                     CharacterPhysics;
        MeshShapeTransformations            PreciseMeshCollision;
        BPLayerInterfaceImpl                BroadPhaseLayer;
        ObjectLayerPairFilterImpl           ObjectObjectLayerFilter;
        MyBodyActivationListener            BodyActivationListen;
        MyContactListener                   ContactListener;

        JPH::PhysicsSystem                  NewPhysicsSystem;
        JPH::ObjectVsBroadPhaseLayerFilter  ObjectFilter;
        JPH::BodyInterface                  *NewPhysicsBodyInterface;
};

#endif