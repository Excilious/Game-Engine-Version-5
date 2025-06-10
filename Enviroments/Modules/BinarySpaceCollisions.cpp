#include "BinarySpaceCollisions.h"

BinarySpaceCollisions::BinarySpaceCollisions(void): FilePointer(NULL) {}

bool BinarySpaceCollisions::TraceBoxBoolean(glm::vec3 Start,glm::vec3 End,glm::vec3 Minimum,glm::vec3 Maximum)
{
    this->TraceType = TYPE_BOX;
    this->TraceMins = Minimum;
    this->TraceMax  = Maximum;
    this->Collided  = false;

    this->Extents   = glm::vec3(
        -this->TraceMins.x > this->TraceMax.x ? -this->TraceMins.x : this->TraceMax.x,
        -this->TraceMins.y > this->TraceMax.y ? -this->TraceMins.y : this->TraceMax.y,
        -this->TraceMins.z > this->TraceMax.z ? -this->TraceMins.z : this->TraceMax.z
    );
    glm::vec3 NewPosition = this->Trace(Start,End);
    return this->Collided;
}

int BinarySpaceCollisions::FindLeaf(glm::vec3 Point)
{
    int Index       = 0;
    float Distance  = 0.0f;

    while (Index >= 0)
    {
        const BSPNode& NewNode      = this->CollisionNode[Index];
        const BSPPlane& NewPlane    = this->CollisionPlane[NewNode.PlaneIndex];
        Distance                    = NewPlane.Normal.x*Point.x+NewPlane.Normal.y*Point.y+NewPlane.Normal.z*Point.z-NewPlane.Distance;
        
        if (Distance >= 0)  {Index = NewNode.Front;}
        else                {Index = NewNode.Back;}
    }
    return ~Index;
}

glm::vec3 BinarySpaceCollisions::CheckNextPosition(glm::vec3 Start,glm::vec3 End)
{
    for (float Height = 1.0f; Height < MAX_STEP_HEIGHT; Height++)
    {
        this->Collided          = false;
        this->TryStep           = false;

        glm::vec3 StepStart     = glm::vec3(Start.x,Start.y+Height,Start.z);
        glm::vec3 StepEnd       = glm::vec3(End.x,Start.y+Height,End.z);
        glm::vec3 StepPosition  = this->Trace(StepStart,StepEnd);

        if (!this->Collided)    {return StepPosition;}
    }
    return Start;
}

glm::vec3 BinarySpaceCollisions::GetCollisionNormal(void)
{
    if (this->TraceType == TYPE_RAY)
    {return this->CollisionNormal;}
    SysPrint("Collision normal provided when using TraceRay! Returning default normal.",2);
    return glm::vec3(0.0f,1.0f,0.0f);
}

glm::vec3 BinarySpaceCollisions::Trace(glm::vec3 Start,glm::vec3 End)
{
    this->TraceRatio = 1.0f;
    this->CheckNode(0,0.0f,1.0f,glm::vec3(Start.x,Start.y,Start.z),End);

    if (this->TraceRatio == 1.0f) {return End;}
    else
    {
        glm::vec3 NewPosition   = Start+((End-Start)*this->TraceRatio);
        glm::vec3 Move          = End-NewPosition;
        float Distance          = glm::dot(Move,this->CollisionNormal);
        glm::vec3 EndPosition   = End-this->CollisionNormal*Distance;
        NewPosition             = this->Trace(NewPosition,EndPosition);

        if (this->CollisionNormal.y > 0.1f || this->Grounded) {this->Grounded = true;}
        else                                                  {this->Grounded = false;}
        return NewPosition;
    }
}

glm::vec3 BinarySpaceCollisions::TraceRay(glm::vec3 Start,glm::vec3 End)
{
    this->TraceType = TYPE_RAY;
    return this->Trace(Start,End);
}

glm::vec3 BinarySpaceCollisions::TraceSphere(glm::vec3 Start,glm::vec3 End,float Radius)
{
    this->TraceType = TYPE_SPHERE;
    this->Collided  = false;
    this->TryStep   = false;
    this->Grounded  = false;
    this->TraceRadius = Radius;
    
    glm::vec3 NewPosition = this->Trace(Start,End);
    if (this->Collided && this->TryStep) {NewPosition = this->CheckNextPosition(NewPosition,End);}
    return NewPosition;
}

glm::vec3 BinarySpaceCollisions::TraceBox(glm::vec3 Start,glm::vec3 End,glm::vec3 Minimum,glm::vec3 Maximum)
{
    this->TraceType = TYPE_BOX;
    this->TraceMins = Minimum;
    this->TraceMax  = Maximum;
    this->Collided  = false;
    this->TryStep   = false;
    this->Grounded  = false;

    this->Extents   = glm::vec3(
        -this->TraceMins.x > this->TraceMax.x ? -this->TraceMins.x : this->TraceMax.x,
        -this->TraceMins.y > this->TraceMax.y ? -this->TraceMins.y : this->TraceMax.y,
        -this->TraceMins.z > this->TraceMax.z ? -this->TraceMins.z : this->TraceMax.z
    );
    glm::vec3 NewPosition = this->Trace(Start,End);
    if (this->Collided && this->TryStep) {NewPosition = this->CheckNextPosition(NewPosition,End);}
    return NewPosition;
}

void BinarySpaceCollisions::GetCollisionData(std::string FileName)
{
    char MagicNumber[4];
    int Version;

    if ((this->FilePointer = fopen(FileName.c_str(),"rb")) == NULL)
    {SysPrint("Failed to open bsp file of "+FileName+" for collision",3);}
    
    fread(&MagicNumber,sizeof(char),4,this->FilePointer);
    fread(&Version,sizeof(int),1,this->FilePointer);
    fread(this->Lumps,sizeof(BSPLump),KMaxLumps,this->FilePointer);

    this->NumberOfNodes                     = this->Lumps[KNodes].Length/sizeof(BSPNode);
    this->CollisionNode                     = new BSPNode[this->NumberOfNodes];
    fseek(this->FilePointer,this->Lumps[KNodes].Offset,SEEK_SET);
    fread(this->CollisionNode,this->NumberOfNodes,sizeof(BSPNode),this->FilePointer);

    this->NumberOfLeafs                     = this->Lumps[KLeafs].Length/sizeof(BSPLeaf);
    this->CollisionLeaf                     = new BSPLeaf[this->NumberOfLeafs];
    fseek(this->FilePointer,this->Lumps[KLeafs].Offset,SEEK_SET);
    fread(this->CollisionLeaf,this->NumberOfLeafs,sizeof(BSPLeaf),this->FilePointer);
    int Temporary;

    for (int Index = 0; Index < this->NumberOfLeafs; Index++)
    {
        Temporary = this->CollisionLeaf[Index].MinBounds[1];
        this->CollisionLeaf[Index].MinBounds[1] = this->CollisionLeaf[Index].MinBounds[2];
        this->CollisionLeaf[Index].MinBounds[2] = -Temporary;

        Temporary = this->CollisionLeaf[Index].MaxBounds[1];
        this->CollisionLeaf[Index].MaxBounds[1] = this->CollisionLeaf[Index].MaxBounds[2];
        this->CollisionLeaf[Index].MaxBounds[2] = -Temporary;
    }

    this->NumberOfPlanes                    = this->Lumps[KPlanes].Length/sizeof(BSPPlane);
    this->CollisionPlane                    = new BSPPlane[this->NumberOfPlanes];
    fseek(this->FilePointer,this->Lumps[KPlanes].Offset,SEEK_SET);
    fread(this->CollisionPlane,this->NumberOfPlanes,sizeof(BSPPlane),this->FilePointer);

    for (int Index = 0; Index < this->NumberOfPlanes; Index++)
    {
        float Temporary = this->CollisionPlane[Index].Normal.y;
        this->CollisionPlane[Index].Normal.y = this->CollisionPlane[Index].Normal.z;
        this->CollisionPlane[Index].Normal.z = -Temporary;
    }

    this->NumberOfBrushes                   = this->Lumps[KBrushes].Length/sizeof(BSPBrush);
    this->CollisionBrushes                  = new BSPBrush[this->NumberOfBrushes];
    fseek(this->FilePointer,this->Lumps[KBrushes].Offset,SEEK_SET);
    fread(this->CollisionBrushes,this->NumberOfBrushes,sizeof(BSPBrush),this->FilePointer);

    this->NumberOfBrushSides                = this->Lumps[KBrushSides].Length/sizeof(BSPBrushSide);
    this->CollisionBrushSides               = new BSPBrushSide[this->NumberOfBrushSides];
    fseek(this->FilePointer,this->Lumps[KBrushSides].Offset,SEEK_SET);
    fread(this->CollisionBrushSides,this->NumberOfBrushSides,sizeof(BSPBrushSide),this->FilePointer);

    this->NumberOfTexturesCollision         = this->Lumps[KTextures].Length/sizeof(BSPTexture);
    this->CollisionTextures                 = new BSPTexture[this->NumberOfTexturesCollision];
    fseek(this->FilePointer,this->Lumps[KTextures].Offset,SEEK_SET);
    fread(this->CollisionTextures,this->NumberOfTexturesCollision,sizeof(BSPTexture),this->FilePointer);

    this->NumberOfLeafBrushes               = this->Lumps[KLeafBrushes].Length/sizeof(int);
    this->LeafBrushes                       = new int[this->NumberOfLeafBrushes];
    fseek(this->FilePointer,this->Lumps[KLeafBrushes].Offset,SEEK_SET);
    fread(this->LeafBrushes,this->NumberOfLeafBrushes,sizeof(int),this->FilePointer);
    fclose(this->FilePointer);
}

void BinarySpaceCollisions::CheckBrush(BSPBrush *NewBrush,glm::vec3 Start,glm::vec3 End)
{
    float StartRatio    = -1.0f;
    float EndRatio      = 1.0f;
    bool StartOut       = false;

    for (int Index = 0; Index < NewBrush->NumberSide; Index++)
    {
        BSPBrushSide *NewBrushSide  = &this->CollisionBrushSides[NewBrush->FirstSide+Index];
        BSPPlane *NewPlane          = &this->CollisionPlane[NewBrushSide->PlaneIndex];

        float Offset = 0.0f;
        if (this->TraceType == TYPE_SPHERE) {Offset = this->TraceRadius;}

        float StartDistance = glm::dot(Start,NewPlane->Normal)-(NewPlane->Distance+Offset);
        float EndDistance   = glm::dot(End,NewPlane->Normal)-(NewPlane->Distance+Offset);

        glm::vec3 VectorOffset = glm::vec3(0.0f,0.0f,0.0f);
        if (this->TraceType == TYPE_BOX)    
        {
            VectorOffset.x = ((NewPlane->Normal.x < 0) ? this->TraceMax.x : this->TraceMins.x);
            VectorOffset.y = ((NewPlane->Normal.y < 0) ? this->TraceMax.y : this->TraceMins.y);
            VectorOffset.z = ((NewPlane->Normal.z < 0) ? this->TraceMax.z : this->TraceMins.z);

            StartDistance   = glm::dot(Start+VectorOffset,NewPlane->Normal)-NewPlane->Distance;
            EndDistance     = glm::dot(End+VectorOffset,NewPlane->Normal)-NewPlane->Distance;
        }
        if (StartDistance > 0)                      {StartOut = true;}
        if (StartDistance > 0 && EndDistance > 0)   {return;}
        if (StartDistance <= 0 && EndDistance <= 0) {continue;}

        if (StartDistance > EndDistance)
        {
            float Ratio1 = (StartDistance-KEPSILON)/(StartDistance-EndDistance);
            if (Ratio1>StartRatio)
            {
                StartRatio              = Ratio1;
                this->Collided          = true;
                this->CollisionNormal   = NewPlane->Normal;

                if ((Start.x != End.x || Start.z != End.z) && NewPlane->Normal.y != 1)  {this->TryStep = true;}
                if (this->CollisionNormal.y > 0.2f)                                     {this->Grounded = true;}
            }
        }
        else
        {
            float Ratio = (StartDistance+KEPSILON)/(StartDistance-EndDistance);
            if (Ratio < EndRatio) {EndRatio = Ratio;}
        }
    }
    
    if (StartOut == false) {return;}
    if (StartRatio < EndRatio)
    {
        if (StartRatio > -1 && StartRatio < this->TraceRatio)
        {
            if (StartRatio < 0) {StartRatio = 0;}
            this->TraceRatio = StartRatio;
        }
    }
}

void BinarySpaceCollisions::CheckNode(int NodeIndex,float StartRatio,float EndRatio,glm::vec3 Start,glm::vec3 End)
{
    if (NodeIndex < 0)
    {
        BSPLeaf *NewLeaf = &this->CollisionLeaf[-(NodeIndex+1)];
        for (int Index = 0; Index < NewLeaf->NumLeafBrush; Index++)
        {
            int BrushIndex      = this->LeafBrushes[NewLeaf->FirstLeafBrush+Index];
            BSPBrush *NewBrush  = &this->CollisionBrushes[BrushIndex];
            if ((NewBrush->NumberSide > 0) && (this->CollisionTextures[NewBrush->TextureIndex].Contents & 1))
            {this->CheckBrush(NewBrush,Start,End);}
        }
        return;
    }

    BSPNode *NewNode        = &this->CollisionNode[NodeIndex];
    BSPPlane *NewPlane      = &this->CollisionPlane[NewNode->PlaneIndex];
    NewPlane->Normal        = glm::normalize(NewPlane->Normal);
    float StartDistance     = glm::dot(Start, NewPlane->Normal) - NewPlane->Distance;
    float EndDistance       = glm::dot(End, NewPlane->Normal) - NewPlane->Distance;    
    float Offset            = 0.0f;

    if (this->TraceType == TYPE_SPHERE)     {Offset = this->TraceRadius;}
    else if (this->TraceType == TYPE_BOX)   {Offset = (float)(fabs(this->Extents.x*NewPlane->Normal.x)+fabs(this->Extents.y*NewPlane->Normal.y)+fabs(this->Extents.z*NewPlane->Normal.z));}

    if (StartDistance >= Offset && EndDistance >= Offset)
    {this->CheckNode(NewNode->Front,StartDistance,EndDistance,Start,End);}
    else if (StartDistance < -Offset && EndDistance < -Offset)
    {this->CheckNode(NewNode->Back,StartDistance,EndDistance,Start,End);}
    else
    {
        float Ratio         = 1.0f;
        float Ratio2        = 0.0f;
        float MiddleRatio   = 0.0f;
        float Denominator   = (StartDistance-EndDistance);
        int Side            = NewNode->Front;
        glm::vec3 Middle;

        if (StartDistance < EndDistance)
        {
            Side = NewNode->Back;
            float InverseDistance = 1.0f/Denominator;
            Ratio   = (StartDistance-Offset-KEPSILON)*InverseDistance;
            Ratio2  = (StartDistance+Offset+KEPSILON)*InverseDistance;
        }
        else if (StartDistance > EndDistance)
        {
            float InverseDistance = 1.0f/Denominator;
            Ratio   = (StartDistance+Offset+KEPSILON)*InverseDistance;
            Ratio2  = (StartDistance-Offset-KEPSILON)*InverseDistance;
        }

        if (Ratio < 0.0f)       {Ratio = 0.0f;}
        else if (Ratio > 1.0f)  {Ratio = 1.0f;}
        if (Ratio2 < 0.0f)      {Ratio2 = 0.0f;}
        else if (Ratio2 > 1.0f) {Ratio2 = 1.0f;}

        MiddleRatio = StartRatio+((EndRatio-StartRatio)*Ratio);
        Middle      = Start+((End-Start)*Ratio);
        this->CheckNode(Side,StartRatio,MiddleRatio,Start,Middle);

        MiddleRatio = StartRatio+((EndRatio-StartRatio)*Ratio2);
        Middle      = Start+((End-Start)*Ratio2);

        if (Side == NewNode->Back)
        {this->CheckNode(NewNode->Front,MiddleRatio,EndRatio,Middle,End);}
        else
        {this->CheckNode(NewNode->Back,MiddleRatio,EndRatio,Middle,End);}
    }
}

void BinarySpaceCollisions::DestroyBSPCollisions(void)
{
    if (this->CollisionNode)                {delete[] this->CollisionNode; this->CollisionNode = NULL;}
    if (this->CollisionPlane)               {delete[] this->CollisionPlane; this->CollisionPlane = NULL;}
    if (this->CollisionTextures)            {delete[] this->CollisionTextures; this->CollisionTextures = NULL;}
    if (this->CollisionBrushes)             {delete[] this->CollisionBrushes; this->CollisionBrushes = NULL;}
    if (this->CollisionBrushSides)          {delete[] this->CollisionBrushSides; this->CollisionBrushSides = NULL;}
    if (this->CollisionLeaf)                {delete[] this->CollisionLeaf; this->CollisionLeaf = NULL;}
}