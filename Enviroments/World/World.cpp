#include "World.h"

World::World(): LevelIndex(0) {}

std::vector<float> World::GetSceneVertices(Parser* CurrentParser)
{
    std::vector<float> Vertices;
    if (this->VertexCache.find(this->LevelIndex) != this->VertexCache.end()) {return this->VertexCache[this->LevelIndex];}

    std::string VerticesData  = CurrentParser->GetGlobalDatapackData(this->NewJson["MapData"][this->LevelIndex]["Vertices"]);
    std::istringstream StringStream(VerticesData);
    std::string NewToken;
    float Vertex;
    if (VerticesData == "")          {SysPrint("Failed to get vertices data for map index"+std::to_string(this->LevelIndex),2); return Vertices;}
    while (std::getline(StringStream,NewToken,','))
    {
        try                                     {Vertices.push_back(std::stof(NewToken));} 
        catch (const std::invalid_argument&)    {SysPrint("Invalid or NAN for vertex "+NewToken,2);};
    }

    if (this->VertexCache.find(this->LevelIndex) == this->VertexCache.end()) {this->VertexCache.insert({this->LevelIndex,Vertices});}
    return Vertices;
}

std::vector<unsigned int> World::GetSceneIndices(Parser* CurrentParser)
{
    std::vector<unsigned int> Indices; 
    if (this->IndicesCache.find(this->LevelIndex) != this->IndicesCache.end()) {return this->IndicesCache[this->LevelIndex];}

    std::string IndicesData = CurrentParser->GetGlobalDatapackData(this->NewJson["MapData"][this->LevelIndex]["Indices"]);
    std::istringstream StringStream(IndicesData);
    std::string NewToken;
    unsigned int Index;
    if (IndicesData == "")          {SysPrint("Failed to get indices data for map index"+std::to_string(this->LevelIndex),2); return Indices;}
    while (std::getline(StringStream,NewToken,','))
    {
        try                                     {Indices.push_back(std::stoul(NewToken));} 
        catch (const std::invalid_argument&)    {SysPrint("Invalid or NAN for index "+NewToken,2);};
    }

    if (this->IndicesCache.find(this->LevelIndex) == this->IndicesCache.end())  {this->IndicesCache.insert({this->LevelIndex,Indices});}
    return Indices;
}

void World::GenerateWorld(GLFWwindow* EngineWindow,Parser* WorldParser,float Near,float Far)
{
    if (WorldParser->GetGlobalDatapackData("World.canv") == "") {SysPrint("World.canv isnt found within the datapack or is corrupt.",3); return;}

    std::string     CanvasList = WorldParser->GetGlobalDatapackData("World.canv");
    unsigned int    CanvasType = WorldParser->QueryDataTypeContent("World.canv");

    if (CanvasType == DT_CANVAS) {this->NewJson = nlohmann::json::parse(CanvasList);}
    for (int Index = 0; Index < this->NewJson["Entity"].size(); Index++)
    {
        glm::vec3 Position  = glm::vec3(this->NewJson["Entity"][Index]["Position"][0],this->NewJson["Entity"][Index]["Position"][1],this->NewJson["Entity"][Index]["Position"][2]);
        glm::vec3 Scale     = glm::vec3(this->NewJson["Entity"][Index]["Scale"][0],this->NewJson["Entity"][Index]["Scale"][1],this->NewJson["Entity"][Index]["Scale"][2]);
        glm::quat Rotation  = glm::quat(this->NewJson["Entity"][Index]["Rotation"][0],this->NewJson["Entity"][Index]["Rotation"][1],this->NewJson["Entity"][Index]["Rotation"][2],this->NewJson["Entity"][Index]["Rotation"][3]);

        this->NewInstance.GenerateStaticEntity(WorldParser,
            this->NewJson["Entity"][Index]["ModelName"],
            this->NewJson["Entity"][Index]["MeshReference"],
            WorldParser->GetGlobalDatapackData(this->NewJson["Entity"][Index]["ShaderVert"]),
            WorldParser->GetGlobalDatapackData(this->NewJson["Entity"][Index]["ShaderFrag"]),
            Position,Scale,Rotation,this->NewJson["Entity"][Index]["TextureReference"],
            glm::vec2(this->NewJson["Entity"][Index]["TextureScale"][0],this->NewJson["Entity"][Index]["TextureScale"][1])
        );
    }

    this->WeaponViewport.CreateViewport(EngineWindow,WorldParser,this->NewJson);
} 

void World::RenderWorld(GLFWwindow* EngineWindow,Player* EnginePlayer,Camera *EngineCamera,float Near,float Far,float FieldOfView,unsigned int ReflectanceTexture,float DeltaTime,float PlayerSpeed)
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    this->NewInstance.RenderEntities(EngineCamera,Near,Far,FieldOfView,EngineCamera->CameraMatrix);
    this->WeaponViewport.RenderViewport(EngineWindow,EnginePlayer,EngineCamera,Near,Far,FieldOfView,PlayerSpeed);
}

void World::DestroyWorld(void) {this->NewInstance.DestroyAll();}