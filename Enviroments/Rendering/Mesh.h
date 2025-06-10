#ifndef __MESH_HEADER__
#define __MESH_HEADER__

#include "../Dependancies/glm/glm.hpp"
#include "../Dependancies/jsonimport/json.h"
#include "../Common/Parser.h"
#include "Buffers/VertexBuffer.h"
#include <vector>
#include <string>

class Mesh
{
    public:
                                    Mesh(void);
        std::vector<float>          GetFloats(nlohmann::json Accessor);
        void                        GenerateMesh(unsigned int ScenePositionValueIndex);
        void                        LoadMeshUsingGLTF(Parser* EngineParser,std::string MeshSource);
        void                        TraverseModelNodes(unsigned int NextNode,glm::mat4 Matrix = glm::mat4(1.0f));

        std::vector<unsigned int>   MeshIndices;
        std::vector<SceneVertex>    MeshVertices;
        std::vector<float>          FloatPositionVectors;
        glm::mat4                   MeshMatrix;
        nlohmann::json              MeshJson;
    private:
        std::vector<unsigned char>  MeshData;
        std::vector<unsigned int>   GetIndices(nlohmann::json Accessor);
        std::vector<glm::vec2>      GroupVector2(std::vector<float> FloatVector);
        std::vector<glm::vec3>      GroupVector3(std::vector<float> FloatVector);
        std::string                 MeshContents;
};

#endif