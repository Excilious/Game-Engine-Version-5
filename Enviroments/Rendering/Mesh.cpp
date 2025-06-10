#include "Mesh.h"

Mesh::Mesh(void) {}

std::vector<glm::vec3> Mesh::GroupVector3(std::vector<float> FloatVector)
{
    std::vector<glm::vec3> Vectors;
    for (int Index = 0; Index < FloatVector.size(); Index)
    {Vectors.push_back(glm::vec3(FloatVector[Index++],FloatVector[Index++],FloatVector[Index++]));}
    return Vectors;
}

std::vector<glm::vec2> Mesh::GroupVector2(std::vector<float> FloatVector)
{
    std::vector<glm::vec2> Vectors;
    for (int Index = 0; Index < FloatVector.size(); Index)
    {Vectors.push_back(glm::vec2(FloatVector[Index++],FloatVector[Index++]));}
    return Vectors;
}

std::vector<unsigned int> Mesh::GetIndices(nlohmann::json Accessor)
{
    std::vector<GLuint> Indices;
    unsigned int BufferViewIndices      = Accessor.value("bufferView",0);
    unsigned int Count                  = Accessor["count"];
    unsigned int AccessorByteOffset     = Accessor.value("byteOffset",0);
    unsigned int ComponentType          = Accessor["componentType"];

    nlohmann::json BufferView           = this->MeshJson["bufferViews"][BufferViewIndices];
    unsigned int ByteOffset             = BufferView["byteOffset"];
    unsigned int StartOfData            = ByteOffset+AccessorByteOffset;

    if (ComponentType == 5125)
    {
        for (unsigned int Index = StartOfData; Index < ByteOffset+AccessorByteOffset+Count*4; Index)
        {
            unsigned char Bytes[] = {this->MeshData[Index++],this->MeshData[Index++],this->MeshData[Index++],this->MeshData[Index++]};
            unsigned int Value;
            std::memcpy(&Value,Bytes,sizeof(unsigned int));
            Indices.push_back((GLuint)Value);
        }
    }
    else if (ComponentType == 5123)
    {
        for (unsigned int Index = StartOfData; Index < ByteOffset+AccessorByteOffset+Count*2; Index)
        {
            unsigned char Bytes[] = {this->MeshData[Index++],this->MeshData[Index++]};
            unsigned short Value;
            std::memcpy(&Value,Bytes,sizeof(unsigned short));
            Indices.push_back((GLuint)Value);
        }
    }
    else if (ComponentType == 5122)
    {
        for (unsigned int Index = StartOfData; Index < ByteOffset+AccessorByteOffset+Count*2; Index)
        {
            unsigned char Bytes[] = {this->MeshData[Index++],this->MeshData[Index++]};
            short Value;
            std::memcpy(&Value,Bytes,sizeof(short));
            Indices.push_back((GLuint)Value);
        }
    }
    return Indices;
}

std::vector<float> Mesh::GetFloats(nlohmann::json Accessor)
{
    unsigned int BufferViewIndex        = Accessor.value("bufferView",1);
    unsigned int Count                  = Accessor["count"];
    unsigned int AccessorByteOffset     = Accessor.value("byteOffset",0);

    std::vector<float> FloatVector;
    std::string Type                    = Accessor["type"];
    nlohmann::json BufferView           = this->MeshJson["bufferViews"][BufferViewIndex];
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
        unsigned char Bytes[] = {this->MeshData[Index++],this->MeshData[Index++],this->MeshData[Index++],this->MeshData[Index++]};
        std::memcpy(&Value,Bytes,sizeof(float)); 
        FloatVector.push_back(Value);
    }
    return FloatVector;
}

void Mesh::TraverseModelNodes(unsigned int NextNode,glm::mat4 Matrix)
{
    nlohmann::json NewNode = this->MeshJson["nodes"][NextNode];
    if (NewNode.find("mesh") != NewNode.end())      {this->GenerateMesh(NewNode["mesh"]);}
    if (NewNode.find("children") != NewNode.end())  {for (unsigned int Index = 0; Index < NewNode["children"].size(); Index++) {this->TraverseModelNodes(NewNode["children"][Index],glm::mat4(1.0f));}}
}

void Mesh::LoadMeshUsingGLTF(Parser* EngineParser,std::string SceneSource)
{
    this->MeshContents  = EngineParser->GetGlobalDatapackData(SceneSource);
    this->MeshJson      = nlohmann::json::parse(this->MeshContents);
    this->MeshData      = EngineParser->GetURIData(this->MeshContents);
    this->TraverseModelNodes(0);
}

void Mesh::GenerateMesh(unsigned int ScenePositionValueIndex)
{
    unsigned int ScenePositionAccessorIndex = this->MeshJson["meshes"][ScenePositionValueIndex]["primitives"][0]["attributes"]["POSITION"];
    unsigned int SceneNormalAccessorIndex   = this->MeshJson["meshes"][ScenePositionValueIndex]["primitives"][0]["attributes"]["NORMAL"];
    unsigned int SceneTextureAccessorIndex  = this->MeshJson["meshes"][ScenePositionValueIndex]["primitives"][0]["attributes"]["TEXCOORD_0"];
    unsigned int SceneIndicesAccessorIndex  = this->MeshJson["meshes"][ScenePositionValueIndex]["primitives"][0]["indices"];

    std::vector<float> PositionVector       = this->GetFloats(this->MeshJson["accessors"][ScenePositionAccessorIndex]);
    std::vector<float> NormalVector         = this->GetFloats(this->MeshJson["accessors"][SceneNormalAccessorIndex]);
    std::vector<float> TextureVector        = this->GetFloats(this->MeshJson["accessors"][SceneTextureAccessorIndex]);

    std::vector<glm::vec3> ScenePositions   = this->GroupVector3(PositionVector);
    std::vector<glm::vec3> SceneNormals     = this->GroupVector3(NormalVector);
    std::vector<glm::vec2> SceneTexture     = this->GroupVector2(TextureVector);
    this->MeshIndices                       = this->GetIndices(this->MeshJson["accessors"][SceneIndicesAccessorIndex]);
    this->FloatPositionVectors              = PositionVector;

    for (int Index = 0; Index < PositionVector.size(); Index++)
    {this->MeshVertices.push_back(SceneVertex{ScenePositions[Index],SceneNormals[Index],glm::vec3(1.0f,0.0f,0.0f),SceneTexture[Index]});}
}

