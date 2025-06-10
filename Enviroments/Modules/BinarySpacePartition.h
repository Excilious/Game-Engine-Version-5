#ifndef __BINARY_SPACE_PARTITIONING_HEADER__
#define __BINARY_SPACE_PARTITIONING_HEADER__

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#define MAX_TEXTURES    1000
#define FACE_POLYGON    1
#define GL_ANISOPE      0x84FF

#include <cstring>
#include <filesystem>
#include <map>
#include <stack>
#include <string>
#include <vector>
#include <filesystem>
#include <regex>

#include "../../Dependancies/glad/glad.h"
#include "../../Dependancies/glm/glm.hpp"
#include "../../Dependancies/glm/gtc/matrix_transform.hpp"
#include "../../Dependancies/glm/gtx/rotate_vector.hpp"
#include "../../Dependancies/glm/gtx/vector_angle.hpp"
#include "../Common/Parser.h"
#include "../Rendering/Lighting.h"

typedef unsigned char Byte;
struct BSPPlane
{
    glm::vec3       Normal;
    float           Distance;
};

struct BSPVisData
{
	int             NumberOfClusters;
	int             CharacterPerCluster;
	char            *BitSets;
};

struct BSPNode
{
    int             PlaneIndex;
    int             Front;
    int             Back;
    int             MinBounds[3];
    int             MaxBounds[3];
};

struct BSPLeaf
{
    int             Cluster;
    int             Area;
    int             MinBounds[3];
    int             MaxBounds[3];
    int             FirstLeafFace;
    int             NumLeafFace;
    int             FirstLeafBrush;
    int             NumLeafBrush;
};

struct BSPBrush
{
    int             FirstSide;
    int             NumberSide;
    int             TextureIndex;
};

struct BSPBrushSide
{
    int             PlaneIndex;
    int             TextureIndex;
};

struct BSPHeader
{
    char            StringID[4];
    int             Version;
};

struct BSPLump
{
    int             Offset;
    int             Length;
};

struct BSPVertex
{
    glm::vec3       Position;
    glm::vec2       TextureCoord;
    glm::vec2       LightMapCoord;
    glm::vec3       Normal;
    Byte            Colour[4];
};

struct BSPFace
{
    int             TextureID;
    int             Effect;
    int             Type;
    int             StartVertexIndex;
    int             NumberOfVertex;
    int             StartIndex;
    int             NumberOfIndices;
    int             LightMapID;
    int             LightMapCorner[2];
    int             MapSize[2];
    glm::vec3       MapPosition;
    glm::vec3       MapVectors[2];
    glm::vec3       Normal;
    int             Size[2];
};

struct BSPTexture
{
    char            StringName[64];
    int             Flags;
    int             Contents;
};

struct BSPLightmap
{
    Byte            ImageBits[128][128][3];
};

struct FaceBuffers
{
    unsigned int    VertexBuffers;
    unsigned int    VertexArray;
    unsigned int    ElementBuffers;
};

struct FaceBufferArray
{std::map<int,FaceBuffers> FaceBufferIndex;};

struct RenderBuffers
{
    std::map<int,std::vector<GLfloat>>          FaceVertexBuffers;
    std::map<int,std::vector<GLuint>>           FaceVertexIndex;
    std::map<int,std::vector<GLfloat>>          FaceNormals;
    std::map<int,std::string>                   TextureVector;
    std::vector<BSPLightmap>                    LightMaps;
    std::map<GLuint,GLuint>                     TextureOptimised;
    std::map<GLuint,GLuint>                     LightMapOptimised;
};

struct TraceResult
{
    bool        AllSolid;
    bool        StartSolid;
    bool        Hit;
    float       Fraction;
    glm::vec3   HitPoint;
    glm::vec3   HitNormal;
};

enum Lumps
{
    KEntites = 0,
    KTextures,
    KPlanes,
    KNodes,
    KLeafs,
    KLeafFaces,
    KLeafBrushes,
    KModels,
    KBrushes,
    KBrushSides,
    KVertices,
    KIndices,
    KShaders,
    KFaces,
    KLightmaps,
    KLightVolumes,
    KVisData,
    KMaxLumps
};

class BinarySpacePartition
{
    public:
                                        BinarySpacePartition(void);
        void                            LoadBinarySpacePartition(std::string FileName,Shader* BinaryShader);
        void                            SetupBinarySpacePartition(Parser* EngineParser);
        void                            RenderAllFaces(Shader* BinaryShader,const glm::vec3 CameraPosition,glm::mat4 ViewMatrix,glm::mat4 ProjectionMatrix);
        void                            CheckNode(int NodeIndex,float StartRatio,float EndRatio,glm::vec3 Start,glm::vec3 End);
        void                            CheckBrush(BSPBrush *NewBrush,glm::vec3 Start,glm::vec3 End);
        void                            DestroyScene(void);
        int                             IsClusterVisible(int Current,int Test);
        int                             FindLeaf(glm::vec3 Point);

        std::vector<glm::vec3>          LightPositions;
        std::vector<glm::vec3>          GetEntityLights(const std::string& EntityText);
        glm::vec3                       SpawnPlayerInBSP(void);
        glm::mat4                       ModelMatrix;
        FILE*                           FilePointer;
        BSPLump                         Lumps[KMaxLumps] = {0};
    private:
        void                            RenderOneFace(int Index,unsigned int Program,glm::mat4 ViewMatrix,glm::mat4 ProjectionMatrix);
        void                            GenerateTextures(Parser* EngineParser);
        void                            GenerateLightMaps(void);
        void                            GetPlayerSpawnPositions(void);
        void                            BuildVertexBuffer(void);
        void                            CreateVertexBuffer(int Index);
        void                            CreateIndices(int Index);
        void                            CreateRenderBuffers(int Index);
        void                            Debug(int Index);

        int                             NumberOfVerts;
        int                             NumberOfFaces;
        int                             NumberOfIndices;
        int                             NumberOfTextures;
        int                             NumberOfLightmaps;
        int                             NumberOfVisibleFaces;
        int                             NumberOfNodes;
        int                             NumberOfLeafs;
        int                             NumberOfLeafFace;
        int                             NumberOfPlanes;
       
        int                             SkipIndices;
        int                             BinaryRecursion;
        int*                            Indices;
        char                            TNameCache[MAX_TEXTURES][64];

        BSPHeader                       Header = {0};
        Lighting                        NewEnviromentLighting;
        BSPVisData                      VisibleData;

        int                             *LeafFace;
        GLuint*                         BSPTextures;
        GLuint*                         LightmapTexturesID;
        GLuint                          MissingLightmapIDs;
        GLuint                          MissingIDs;
        
        std::vector<std::string>        TName;
        std::vector<glm::vec3>          PlayerSpawnPositions;
        std::vector<BSPFace*>           DrawableFaces;

        RenderBuffers                   RendBuffer;
        FaceBufferArray                 FaceBufferArray;

        unsigned int                   *FaceBitSet;
        int                             FaceBitSize;
        BSPPlane*                       ScenePlane;
        BSPVertex*                      Vertices;
        BSPNode*                        SceneNode;
        BSPLeaf*                        SceneLeaf;
        BSPFace*                        Faces;
};

#endif