#include "Actor.h"

Actor::Actor(void): CurrentOwnership(OWNER_NULL) {}

void Actor::CreateActor(const std::string ActorName,const std::string ModelFile,Parser* NewParser,const glm::vec3 Position,const glm::quat Rotation,const glm::vec3 Scale,const std::string TextureReference,const glm::vec2 TextureSize)
{
    this->CreateEntity(ActorName,ModelFile,NewParser,Position,Rotation,Scale,TextureReference,TextureSize);
    this->GetAllAnimationInstances(this->MeshJson);
}

BoundingBoxActor Actor::GetMinimumMaximum(void)
{
    BoundingBoxActor NewBoundingActor;
    glm::vec3 MinimumValues = glm::vec3(FLT_MIN,FLT_MIN,FLT_MIN);
    glm::vec3 MaximumValues = glm::vec3(-FLT_MAX,-FLT_MAX,-FLT_MAX);

    for (const auto& Vertex: this->Vertices)
    {
        MinimumValues = glm::min(MinimumValues,Vertex);
        MaximumValues = glm::max(MaximumValues,Vertex);
    }
    NewBoundingActor.Minimum = MinimumValues;
    NewBoundingActor.Maximum = MaximumValues;
    return NewBoundingActor;
}
