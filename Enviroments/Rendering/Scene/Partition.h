#ifndef __PARTITION_HEADER__
#define __PARTITION_HEADER__

#include "../Dependancies/glm/glm.hpp"
#include "../Dependancies/jsonimport/json.h"
#include "../Buffers/VertexBuffer.h"
#include "../../Common/Parser.h"
#include <vector>
#include <string>

class Partition
{
    public:
                                    Partition(void);
        std::vector<float>          GetFloats(nlohmann::json Accessor);
        void                        GenerateScene(unsigned int ScenePositionValueIndex);
        void                        LoadPartitionUsingGLTF(Parser* EngineParser,std::string SceneSource);

        std::vector<unsigned int>   PartitionIndices;
        std::vector<SceneVertex>    PartitionVertices;
        std::vector<float>          FloatPositionVectors;
        glm::mat4                   MeshMatrix;
    private:
        void                        GetPredefinedMatrix(nlohmann::json Accessor);
        nlohmann::json              SceneJSON;
        std::string                 SceneContents;
        std::vector<unsigned char>  SceneData;
        std::vector<unsigned int>   GetIndices(nlohmann::json Accessor);
        std::vector<glm::vec2>      GroupVector2(std::vector<float> FloatVector);
        std::vector<glm::vec3>      GroupVector3(std::vector<float> FloatVector);
};

#endif