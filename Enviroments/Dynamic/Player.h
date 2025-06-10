#ifndef __PLAYER_HEADER__
#define __PLAYER_HEADER__

#include "../Humanoid/ActorLogic.h"
#include "../Common/Parser.h"
#include "../Rendering/Lighting.h"
#include "../Modules/BinarySpaceCollisions.h"
#include "../Modules/BinarySpacePartition.h"

struct PlayerAttributes
{
    float   PlayerHealth = 100;
    bool    PlayerDied   = false; 
};

class Player: public ActorLogic
{
    public:
                            Player(void);
        PlayerAttributes    LocalPlayerAttribute;
        void                LoadPlayer(Parser* EngineParser,Camera* EngineCamera);
        void                Movement(GLFWwindow* InputWindow,const glm::vec3 Orientation,const glm::vec3 Position,BinarySpaceCollisions* NewCollisions,BinarySpacePartition* EnginePartition,const float DeltaTime);
        void                CheckPlayerAttributes(GLFWwindow* InputWindow,BinarySpacePartition* EnginePartition);
        void                Destroy(void);

        float               PlayerSpeed         = 0.2f;
        float               PlayerAcceleration  = 0.01f;
        float               PlayerFriction      = 15.6f;
        float               PlayerDeceleration  = 0.0005f;
        glm::vec3           PlayerPosition;
        glm::vec3           PlayerVelocity;
    private:
        bool                IsPlayerOnGround;
        Shader              PlayerCharacterShader;
        glm::quat           PlayerRotation;
        glm::vec3           PlayerScale;
        glm::vec3           PlayerOrientation;
};

#endif