#ifndef __ANIMATION_HEADER__
#define __ANIMATION_HEADER__

#include <string>
#include <vector>
#include "../../Dependancies/jsonimport/json.h"
#include "../../Dependancies/glm/glm.hpp"

struct Node
{
    std::string         NodeName;
    std::vector<int>    Children;
    int                 Parent      = -1;
    int                 JointIndex  = -1;
    glm::mat4           LocalMatrix;
    glm::mat4           GlobalMatrix;
    glm::mat4           InverseBindMatrix;
};

struct AnimationSampler
{
    std::vector<float>      Times;
    std::vector<glm::vec3>  Translation;
    std::vector<glm::quat>  Rotations;
};

class Animation
{
    public:
                                                    Animation(void);
        void                                        GetAllAnimationInstances(nlohmann::json ModelJSON);
        void                                        GetAnimationSkin(void);
        void                                        ParseAnimationChannel(void);
    private:
        std::vector<Node>                           Nodes;
        nlohmann::json                              AnimationJSON;
        std::unordered_map<int,AnimationSampler>    Animations;
};

#endif