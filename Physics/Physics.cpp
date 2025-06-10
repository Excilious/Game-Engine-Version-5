#include "Physics.h"

using namespace JPH::literals;

static void TraceImpl(const char *inFMT, ...)
{
	va_list list;
	va_start(list, inFMT);
	char buffer[1024];
	vsnprintf(buffer, sizeof(buffer), inFMT, list);
	va_end(list);
    SysPrint(buffer,2);
}

Physics::Physics() {}

void Physics::StartPhysicsEngine(std::vector<float> Vertices,std::vector<unsigned int> Indices) 
{
    SysPrint("Starting Jolt Engine...",1);
    JPH::RegisterDefaultAllocator();
    JPH::Trace = TraceImpl;

    JPH::Factory::sInstance = new JPH::Factory();
    JPH::RegisterTypes();

    JPH::TempAllocatorImpl 	 TemporaryAllocator(10*1024*1024);
    JPH::JobSystemThreadPool JobSystem(JPH::cMaxPhysicsJobs,JPH::cMaxPhysicsBarriers,std::thread::hardware_concurrency()-1);

	this->NewPhysicsSystem.Init(1024,0,1024,1024,this->BroadPhaseLayer,this->ObjectFilter,this->ObjectObjectLayerFilter);
    this->NewPhysicsSystem.SetContactListener(&this->ContactListener);
	this->NewPhysicsSystem.SetBodyActivationListener(&this->BodyActivationListen);
    this->NewPhysicsBodyInterface = &NewPhysicsSystem.GetBodyInterface();
    //this->CharacterPhysics.CreateHumanoidBoundCapsule(&this->NewPhysicsSystem);

    this->PreciseMeshCollision.TransformVerticesToMeshShape(Vertices,Indices);
    this->NewPhysicsBodyInterface->CreateAndAddBody(this->PreciseMeshCollision.CreateMeshShape(),JPH::EActivation::DontActivate);
    this->NewPhysicsSystem.OptimizeBroadPhase();
}

void Physics::RenderPhysicsEngine(Camera* EngineCamera)
{
    //this->CharacterPhysics.UpdateHumanoidPosition(EngineCamera->Position);
}

void Physics::DestroyPhysicsEngine()
{
    JPH::UnregisterTypes();
    delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;
}