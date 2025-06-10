#include "Player.h"
#include "../Common/Console.h"

Player::Player(void): IsPlayerOnGround(false),PlayerRotation(glm::quat(0.0f,0.0f,0.0f,1.0f)),PlayerScale(glm::vec3(3.0f,6.0f,3.0f)) {}

void Player::LoadPlayer(Parser* EngineParser, Camera* EngineCamera) {
    this->CreateActor("PlayerHumanoid","Ranger.gltf", EngineParser, 
        glm::vec3(0.0f, 0.0f, 0.0f),glm::quat(0.0f, 0.0f, 0.0f, 0.0f),glm::vec3(0.02f, 0.03f, 0.02f),
        "RangerTex.imgbuf",glm::vec2(256,256)
    );
    this->PlayerCharacterShader.CreateNewShader(
        EngineParser->GetGlobalDatapackData("Default.vert").c_str(),
        EngineParser->GetGlobalDatapackData("Default.frag").c_str(),
        true);
}

void Player::CheckPlayerAttributes(GLFWwindow* InputWindow,BinarySpacePartition* EnginePartition)
{
    if (this->LocalPlayerAttribute.PlayerHealth <= 0)
    {
        this->LocalPlayerAttribute.PlayerHealth = 0;
        this->LocalPlayerAttribute.PlayerDied   = true;
        if (glfwGetKey(InputWindow, GLFW_KEY_ENTER) == GLFW_PRESS)
        {
            this->PlayerPosition                    = EnginePartition->SpawnPlayerInBSP();
            this->LocalPlayerAttribute.PlayerHealth = 100;
            this->LocalPlayerAttribute.PlayerDied   = false;
        }
    }
}

void Player::Movement(GLFWwindow* InputWindow, const glm::vec3 Orientation, const glm::vec3 Position, BinarySpaceCollisions* NewCollisions, BinarySpacePartition* EnginePartition, const float DeltaTime) 
{
    this->CheckPlayerAttributes(InputWindow,EnginePartition);
    if (ConsoleOpen)                            {return;}
    if (this->LocalPlayerAttribute.PlayerDied)  {return;}

    glfwSetInputMode(InputWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glm::vec3 NewMovement   = glm::vec3(0.0f,0.0f,0.0f);
    float MoveSpeed         = this->PlayerSpeed * 0.2f;

    if (glfwGetKey(InputWindow, GLFW_KEY_W) == GLFW_PRESS)                                                  {NewMovement += MoveSpeed * Orientation;}
    if (glfwGetKey(InputWindow, GLFW_KEY_A) == GLFW_PRESS)                                                  {NewMovement += MoveSpeed * -glm::normalize(glm::cross(Orientation, glm::vec3(0.0f, 1.0f, 0.0f)));}
    if (glfwGetKey(InputWindow, GLFW_KEY_S) == GLFW_PRESS)                                                  {NewMovement += MoveSpeed * -Orientation;}
    if (glfwGetKey(InputWindow, GLFW_KEY_D) == GLFW_PRESS)                                                  {NewMovement += MoveSpeed * glm::normalize(glm::cross(Orientation, glm::vec3(0.0f, 1.0f, 0.0f)));}
    if (glfwGetKey(InputWindow, GLFW_KEY_SPACE) == GLFW_PRESS && NewCollisions->Grounded)                   {this->PlayerPosition.y += 60; NewCollisions->Grounded = false;}
    if (glfwGetKey(InputWindow, GLFW_KEY_Q) == GLFW_PRESS)    {this->PlayerPosition.y += 30;}
    if (glfwGetKey(InputWindow, GLFW_KEY_E) == GLFW_PRESS)    {this->PlayerPosition.y -= 30;}

    if (glfwGetKey(InputWindow, GLFW_KEY_B) == GLFW_PRESS)                                                  {this->LocalPlayerAttribute.PlayerHealth -= 10;}

    glm::vec3 HorizontalVelocity    = glm::vec3(this->PlayerVelocity.x,0,this->PlayerVelocity.z);
    float Acceleration              = (NewCollisions->Grounded) ? 200.0f : 100.0f;

    if (glm::length(NewMovement) > 0.0f)
    {
        glm::vec3 DesiredVelocity       = NewMovement*180.0f;
        glm::vec3 VelocityDelta         = DesiredVelocity-HorizontalVelocity;
        glm::vec3 AccelerationVector    = glm::clamp(VelocityDelta,-glm::vec3(Acceleration),glm::vec3(Acceleration));

        HorizontalVelocity              += AccelerationVector*DeltaTime;

        if (glm::length(HorizontalVelocity) > 180.0f)
        {HorizontalVelocity = glm::normalize(HorizontalVelocity) * 180.0f;}
    }
    else if (NewCollisions->Grounded)
    {HorizontalVelocity *= 1.0f / (1.0f + 16.0f * DeltaTime);}

    this->PlayerVelocity.x      = HorizontalVelocity.x;
    this->PlayerVelocity.z      = HorizontalVelocity.z;
    this->PlayerOrientation     = Orientation;
    this->PlayerPosition       += this->PlayerVelocity*DeltaTime;
    this->PlayerPosition.y     += this->PlayerVelocity.y*DeltaTime;
    this->PlayerVelocity.y      = 0;
}

void Player::Destroy(void) 
{
    this->PlayerCharacterShader.DestroyShader();
    this->DestroyModel();
}
