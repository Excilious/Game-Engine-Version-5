#include "Instance.h"

Instance::Instance(void) {}

void Instance::GenerateStaticEntity(Parser *EntityParser,std::string ModelName,std::string EntityName,std::string ShaderVert,std::string ShaderFrag,glm::vec3 Position,glm::vec3 Scale,glm::quat Rotation,std::string TextureReference,glm::vec2 TextureSize)
{
    EntityNode  Node;
    Actor       ActorModel;
    Shader      EntityShader;

    ActorModel.CreateActor(ModelName,EntityName,EntityParser,Position,Rotation,Scale,TextureReference,TextureSize);

    EntityShader.CreateNewShader(ShaderVert.c_str(),ShaderFrag.c_str(),true);
    Node.NewEntityInstance = ActorModel;
    Node.ShaderEntity      = EntityShader;
    Node.NewEntityType     = ENT_STATIC;

    this->EntityList.push_back(Node);
}

void Instance::RenderEntities(Camera *EngineCamera,float Near,float Far,float FieldOfView,glm::mat4 CameraMatrix)
{
    for (int Index = 0; Index < this->EntityList.size(); Index++)
    {
        this->WorldEntityLighting.ResolveModelLighting(&this->EntityList[Index].ShaderEntity,EngineCamera->Position,this->EntityList[Index].NewEntityInstance.ModelAmbience,this->EntityList[Index].NewEntityInstance.ModelSpecular);
        this->EntityList[Index].NewEntityInstance.DrawModel(&this->EntityList[Index].ShaderEntity,EngineCamera,Near,Far,FieldOfView,CameraMatrix);
        this->EntityList[Index].ShaderEntity.ActivateShader();
    }
}

void Instance::DestroyAll(void)
{
    for (int Index = 0; Index < this->EntityList.size(); Index++)
    {this->EntityList[Index].NewEntityInstance.DestroyModel();}
}