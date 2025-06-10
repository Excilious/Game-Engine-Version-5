#include "MeshShapeTransformation.h"
#include "../Utilites/BroadPhaseLayerInterface.h"

MeshShapeTransformations::MeshShapeTransformations()
{ }

void MeshShapeTransformations::TransformVerticesToMeshShape(std::vector<float> Vertices,std::vector<unsigned int> Indices)
{
    std::vector<JPH::Float3>    VertexPositions;
    for (int Index = 0; Index < Indices.size(); Index+=3)
    {
        JPH::Float3 NewFloat = JPH::Float3(Vertices[Index+0],Vertices[Index+1],Vertices[Index+2]);
        VertexPositions.push_back(NewFloat);
    }
    for (int PosIndex = 0; PosIndex < VertexPositions.size(); PosIndex+=3)
    {
        JPH::Triangle NewTriangle = JPH::Triangle(VertexPositions[PosIndex+0],VertexPositions[PosIndex+1],VertexPositions[PosIndex+2]);
        this->MeshTriangles.push_back(NewTriangle);
    }
}

JPH::BodyCreationSettings MeshShapeTransformations::CreateMeshShape()
{
    JPH::Ref<JPH::MeshShapeSettings> MeshShape = new JPH::MeshShapeSettings(this->MeshTriangles);
    JPH::Shape::ShapeResult ShapeResult = MeshShape->Create();
    JPH::RefConst<JPH::Shape> NewMeshShape = ShapeResult.Get();
    
    JPH::BodyCreationSettings BodySettings(
        NewMeshShape,
        JPH::RVec3::sZero(),
        JPH::Quat::sIdentity(),
        JPH::EMotionType::Static,
        Layers::NON_MOVING
    );
    return BodySettings;
}