#include "Animation.h"
#include "../Common/Console.h"

Animation::Animation(void) {};

void Animation::GetAllAnimationInstances(nlohmann::json ModelJSON)
{
    this->AnimationJSON = ModelJSON;
    for (int Index = 0; Index < this->AnimationJSON["nodes"].size(); Index++)
    {
        auto& IndexedNode = this->AnimationJSON["nodes"][Index];
        Node NewNode;

        if (IndexedNode.contains("children"))
        {
            for (auto Child: IndexedNode["children"]) {NewNode.Children.push_back(Child);}
        }

        this->Nodes.push_back(NewNode);
    }
}  

void Animation::GetAnimationSkin(void)
{
    auto& Skin                      = this->AnimationJSON["skin"][0];
    std::vector<int> JointIndices   = Skin["joints"].get<std::vector<int>>();
    int AccessorIndex               = Skin["inverseBindMatrices"];
    auto& Accessor                  = this->AnimationJSON["accessors"][AccessorIndex];
    int BufferViewIndex             = Accessor["accessors"][AccessorIndex];
    auto& BufferView                = this->AnimationJSON["bufferViews"][BufferViewIndex];
    int BufferIndex                 = BufferView["buffer"];
    auto& Buffer                    = this->AnimationJSON["buffers"][BufferIndex];

    std::string URI                 = Buffer["uri"];
    // Load binary buffer std::vector<uint8_t> RawData    = 

    size_t Offset                   = BufferView["byteOffset"];
    for (int Index = 0; Index < Accessor["count"]; Index++)
    {
        //glm::mat4 MatrixBuffer
        //Nodes[jointIndices[Index]].InverseBindmatrix;
    }
}

void Animation::ParseAnimationChannel(void)
{
    auto& Animation = this->AnimationJSON["animations"][0];
    for (auto& Channel: Animation["channels"])
    {
        int Node            = Channel["target"]["node"];
        std::string Path    = Channel["target"]["path"];
        int SamplerIndex    = Channel["sampler"];

        auto& Sampler       = Animation["samplers"][SamplerIndex];
        int InputAccessor   = Sampler["input"];
        int OutputAccessor  = Sampler["output"];

        //std::vector<float> Times = LoadFloatBuffer
        if (Path == "translation")
        {
            //std::vector<glm::vec3> Values = LoadVec3Buffer
            //this->Animations[Node].Times = Times
        }
    }
}