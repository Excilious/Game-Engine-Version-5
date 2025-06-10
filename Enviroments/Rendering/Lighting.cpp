#include "Lighting.h"

Lighting::Lighting(void) {}

void Lighting::CreateLight(glm::vec3 Colour,glm::vec3 Position,float Brightness,glm::vec3 Orientation)
{
    LightBlock NewLightBlock;
    NewLightBlock.Orientation   = Orientation;
    NewLightBlock.Brightness    = Brightness;
    NewLightBlock.Colour        = Colour;
    NewLightBlock.Position      = Position;
    this->WorldLights.push_back(NewLightBlock);
}

void Lighting::ResolveModelLighting(Shader* ModelShader,glm::vec3 CameraPosition,float AmbienceStrength,int SpecularStrength)
{
    for (int Index = 0; Index < this->WorldLights.size(); Index++)
    {
        ModelShader->SetVector3(this->WorldLights[Index].Orientation,"Orientation["+std::to_string(Index)+"]");
        ModelShader->SetVector3(this->WorldLights[Index].Position,"LightSource["+std::to_string(Index)+"]");
        ModelShader->SetVector3(this->WorldLights[Index].Colour,"LightColour["+std::to_string(Index)+"]");
        ModelShader->SetVector3(CameraPosition,"CameraPosition");
        ModelShader->SetVector1((float)this->WorldLights.size(),"LightInstances");
        ModelShader->SetVector1(AmbienceStrength,"AmbienceStrength");
        ModelShader->SetVector1(this->WorldLights[Index].Brightness,"Brightness["+std::to_string(Index)+"]");
        ModelShader->SetVector1(0.75f,"LightGloss["+std::to_string(Index)+"]");
        ModelShader->ActivateShader();
    }
}

void Lighting::ResolveBinarySpaceLighting(Shader* ModelShader,std::vector<glm::vec3> LightingPosition,glm::vec3 CameraPosition)
{
    for (int Index = 0; Index < LightingPosition.size(); Index++)
    {
        ModelShader->SetVector3(LightingPosition[Index],"LightPosition["+std::to_string(Index)+"]");
        ModelShader->SetVector3(glm::vec3(1.0f,1.0f,1.0f),"LightColour["+std::to_string(Index)+"]");
        ModelShader->SetVector3(CameraPosition,"CameraPosition");
        ModelShader->SetVector1(LightingPosition.size(),"LightMaxIndex");
        ModelShader->ActivateShader();
    }
}