#ifndef __MESH_SHAPE_TRANSFORMATIONS_HEADER__
#define __MESH_SHAPE_TRANSFORMATIONS_HEADER__

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyInterface.h>

#include <vector>

class MeshShapeTransformations
{
    public:
        MeshShapeTransformations();
        void                        TransformVerticesToMeshShape(std::vector<float> Vertices,std::vector<unsigned int> Indices);
        JPH::BodyCreationSettings   CreateMeshShape();
    private:
        JPH::TriangleList           MeshTriangles;
};

#endif