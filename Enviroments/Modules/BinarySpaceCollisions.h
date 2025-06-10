#ifndef __BINARY_SPACE_COLLISIONS_HEADER__
#define __BINARY_SPACE_COLLISIONS_HEADER__

#include "BinarySpacePartition.h"

#define MAX_STEP_HEIGHT 22.0f
#define TYPE_RAY        0
#define TYPE_SPHERE     1
#define TYPE_BOX        2
#define KEPSILON        0.03125f

class BinarySpaceCollisions
{
    public:
                            BinarySpaceCollisions(void);
        void                DestroyBSPCollisions(void);
        void                GetCollisionData(std::string FileName);
        int                 FindLeaf(glm::vec3 Point);

        bool                TraceBoxBoolean(glm::vec3 Start,glm::vec3 End,glm::vec3 Minimum,glm::vec3 Maximum);
        glm::vec3           CheckNextPosition(glm::vec3 Start,glm::vec3 End);
        glm::vec3           Trace(glm::vec3 Start,glm::vec3 End);
        glm::vec3           TraceRay(glm::vec3 Start,glm::vec3 End);
        glm::vec3           TraceSphere(glm::vec3 Start,glm::vec3 End,float Radius);
        glm::vec3           TraceBox(glm::vec3 Start,glm::vec3 End,glm::vec3 Minimum,glm::vec3 Maximum);
        glm::vec3           GetCollisionNormal(void);

        bool                Grounded;
        BSPPlane*           CollisionPlane;
        BSPNode*            CollisionNode;
    private:
        void                CheckNode(int NodeIndex,float StartRatio,float EndRatio,glm::vec3 Start,glm::vec3 End);
        void                CheckBrush(BSPBrush *NewBrush,glm::vec3 Start,glm::vec3 End);

        BSPLeaf*            CollisionLeaf;
        BSPBrush*           CollisionBrushes;
        BSPBrushSide*       CollisionBrushSides;
        BSPTexture*         CollisionTextures;
        FILE*               FilePointer;
        BSPLump             Lumps[KMaxLumps]    = {0};

        glm::vec3           CollisionNormal;
        glm::vec3           Extents;
        glm::vec3           TraceMins;
        glm::vec3           TraceMax;

        int                 NumberOfTexturesCollision;
        int                 NumberOfPlanes;
        int                 NumberOfNodes;
        int                 NumberOfLeafs;
        int                 NumberOfBrushes;
        int                 NumberOfBrushSides;
        int                 NumberOfLeafBrushes;
        int*                LeafBrushes;

        bool                Collided;
        bool                TryStep;
        float               TraceRatio;
        float               TraceRadius;
        int                 TraceType;

};

#endif