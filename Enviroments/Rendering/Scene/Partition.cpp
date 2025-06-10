#include "Partition.h"

Partition::Partition(void) {}

std::vector<glm::vec3> Partition::GroupVector3(std::vector<float> FloatVector)
{
    std::vector<glm::vec3> Vectors;
    for (int Index = 0; Index < FloatVector.size(); Index)
    {Vectors.push_back(glm::vec3(FloatVector[Index++],FloatVector[Index++],FloatVector[Index++]));}
    return Vectors;
}

std::vector<glm::vec2> Partition::GroupVector2(std::vector<float> FloatVector)
{
    std::vector<glm::vec2> Vectors;
    for (int Index = 0; Index < FloatVector.size(); Index)
    {Vectors.push_back(glm::vec2(FloatVector[Index++],FloatVector[Index++]));}
    return Vectors;
}

std::vector<unsigned int> Partition::GetIndices(nlohmann::json Accessor)
{
    std::vector<GLuint> Indices;
    unsigned int BufferViewIndices      = Accessor.value("bufferView",0);
    unsigned int Count                  = Accessor["count"];
    unsigned int AccessorByteOffset     = Accessor.value("byteOffset",0);
    unsigned int ComponentType          = Accessor["componentType"];

    nlohmann::json BufferView           = this->SceneJSON["bufferViews"][BufferViewIndices];
    unsigned int ByteOffset             = BufferView["byteOffset"];
    unsigned int StartOfData            = ByteOffset+AccessorByteOffset;

    if (ComponentType == 5125)
    {
        for (unsigned int Index = StartOfData; Index < ByteOffset+AccessorByteOffset+Count*4; Index)
        {
            unsigned char Bytes[] = {this->SceneData[Index++],this->SceneData[Index++],this->SceneData[Index++],this->SceneData[Index++]};
            unsigned int Value;
            std::memcpy(&Value,Bytes,sizeof(unsigned int));
            Indices.push_back((GLuint)Value);
        }
    }
    else if (ComponentType == 5123)
    {
        for (unsigned int Index = StartOfData; Index < ByteOffset+AccessorByteOffset+Count*2; Index)
        {
            unsigned char Bytes[] = {this->SceneData[Index++],this->SceneData[Index++]};
            unsigned short Value;
            std::memcpy(&Value,Bytes,sizeof(unsigned short));
            Indices.push_back((GLuint)Value);
        }
    }
    else if (ComponentType == 5122)
    {
        for (unsigned int Index = StartOfData; Index < ByteOffset+AccessorByteOffset+Count*2; Index)
        {
            unsigned char Bytes[] = {this->SceneData[Index++],this->SceneData[Index++]};
            short Value;
            std::memcpy(&Value,Bytes,sizeof(short));
            Indices.push_back((GLuint)Value);
        }
    }
    return Indices;
}

std::vector<float> Partition::GetFloats(nlohmann::json Accessor)
{
    unsigned int BufferViewIndex        = Accessor.value("bufferView",1);
    unsigned int Count                  = Accessor["count"];
    unsigned int AccessorByteOffset     = Accessor.value("byteOffset",0);

    std::vector<float> FloatVector;
    std::string Type                    = Accessor["type"];
    nlohmann::json BufferView           = this->SceneJSON["bufferViews"][BufferViewIndex];
    unsigned int ByteOffset             = BufferView["byteOffset"];
    unsigned int NumberPerVertices;

    if (Type == "SCALAR")       {NumberPerVertices=1;}
    else if (Type == "VEC2")    {NumberPerVertices=2;}
    else if (Type == "VEC3")    {NumberPerVertices=3;}
    else if (Type == "VEC4")    {NumberPerVertices=4;}
    else {SysPrint("Vector Doesn't Exist.",2);}

    unsigned int StartOfData    = ByteOffset + AccessorByteOffset;
    unsigned int LengthOfData   = Count * 4 * NumberPerVertices;

    for (unsigned int Index = StartOfData; Index < StartOfData+LengthOfData; Index)
    {
        float Value;
        unsigned char Bytes[] = {this->SceneData[Index++],this->SceneData[Index++],this->SceneData[Index++],this->SceneData[Index++]};
        std::memcpy(&Value,Bytes,sizeof(float)); 
        FloatVector.push_back(Value);
    }
    return FloatVector;
}

void Partition::LoadPartitionUsingGLTF(Parser* EngineParser,std::string SceneSource)
{
    this->SceneContents = EngineParser->GetGlobalDatapackData(SceneSource);
    this->SceneJSON     = nlohmann::json::parse(this->SceneContents);
    this->SceneData     = EngineParser->GetURIData(this->SceneContents);
    this->GetPredefinedMatrix(this->SceneJSON["nodes"][0]);
    this->GenerateScene(0);
}

void Partition::GetPredefinedMatrix(nlohmann::json Accessor)
{
    glm::vec3 TranslationVector     = glm::vec3(0.0f,0.0f,0.0f);
    glm::quat RotationVector        = glm::quat(0.0f,0.0f,0.0f,0.0f);
    glm::vec3 ScaleVector           = glm::vec3(1.0f,1.0f,1.0f);
    glm::mat4 MatrixTable           = glm::mat4(1.0f);

    if (Accessor.find("translation") != Accessor.end())
    {
        float TranslationValue[3] = {
            Accessor["translation"][0],
            Accessor["translation"][1],
            Accessor["translation"][2]
        };
        TranslationVector = glm::make_vec3(TranslationValue);
    }
    if (Accessor.find("rotation") != Accessor.end())
    {
        float RotationValue[4] = {
            Accessor["rotation"][3],
            Accessor["rotation"][0],
            Accessor["rotation"][1],
            Accessor["rotation"][2]
        };
        RotationVector = glm::make_quat(RotationValue);
    }
    if (Accessor.find("scale") != Accessor.end())
    {
        float ScaleValue[3] = {
            Accessor["scale"][0],
            Accessor["scale"][1],
            Accessor["scale"][2]
        };
        ScaleVector = glm::make_vec3(ScaleValue);
    }
    if (Accessor.find("matrix") != Accessor.end())
    {
        float MatrixValue[16];
        for (unsigned int Index = 0; Index < Accessor["matrix"].size(); Index++)
        {MatrixValue[Index] = (Accessor["matrix"][Index]);}

        MatrixTable = glm::make_mat4(MatrixValue);
    }

    MeshMatrix = glm::translate(glm::mat4(1.0f),TranslationVector) * glm::mat4_cast(RotationVector) * glm::scale(glm::mat4(1.0f),ScaleVector);
    MeshMatrix = MatrixTable*MeshMatrix;
}

void Partition::GenerateScene(unsigned int ScenePositionValueIndex)
{
    unsigned int ScenePositionAccessorIndex = this->SceneJSON["meshes"][ScenePositionValueIndex]["primitives"][0]["attributes"]["POSITION"];
    unsigned int SceneNormalAccessorIndex   = this->SceneJSON["meshes"][ScenePositionValueIndex]["primitives"][0]["attributes"]["NORMAL"];
    unsigned int SceneTextureAccessorIndex  = this->SceneJSON["meshes"][ScenePositionValueIndex]["primitives"][0]["attributes"]["TEXCOORD_0"];
    unsigned int SceneIndicesAccessorIndex  = this->SceneJSON["meshes"][ScenePositionValueIndex]["primitives"][0]["indices"];

    std::vector<float> PositionVector       = this->GetFloats(this->SceneJSON["accessors"][ScenePositionAccessorIndex]);
    std::vector<float> NormalVector         = this->GetFloats(this->SceneJSON["accessors"][SceneNormalAccessorIndex]);
    std::vector<float> TextureVector        = this->GetFloats(this->SceneJSON["accessors"][SceneTextureAccessorIndex]);

    std::vector<glm::vec3> ScenePositions   = this->GroupVector3(PositionVector);
    std::vector<glm::vec3> SceneNormals     = this->GroupVector3(NormalVector);
    std::vector<glm::vec2> SceneTexture     = this->GroupVector2(TextureVector);
    this->PartitionIndices                  = this->GetIndices(this->SceneJSON["accessors"][SceneIndicesAccessorIndex]);
    this->FloatPositionVectors              = PositionVector;
    
    for (int Index = 0; Index < PositionVector.size(); Index++)
    {
        SceneVertex NewVertex;
        NewVertex.Positions = ScenePositions[Index];
        NewVertex.Normals   = SceneNormals[Index];
        NewVertex.Colour    = glm::vec3(1.0f,0.0f,0.0f);
        NewVertex.TextureUV = SceneTexture[Index];
        this->PartitionVertices.push_back(NewVertex);
    }
}

