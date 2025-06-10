#include "Commands.h"
#include "Console.h"
#include <iostream>
#include "../Network/Client/NetworkClient.h"
#include "../Network/Server/NetworkServer.h"
#include "../Interpreter/VirtualMachine.h"
#include "../Audio/AudioController.h"

AudioController NewAudioController;
std::vector<Register> RegisterList = {
    {"cullface",        1},
    {"multisample",     1},
    {"depth",           1},
    {"blend",           1},
    {"vsyncactive",     1},
    {"showfps",         true},
    {"polygon",         false},
    {"exit",            false},
    {"noclip",          false},
    {"exitconfirm",     false},
    {"LightIntensity",  20.0f},
    {"ApplyLightMap",   1.0f},
    {"fov",             120.0f},
    {"near",            0.1f},
    {"far",             100000.0f},
    {"world",           "-1"},
    {"bsp",             "../../Fusion/Resources/Maps/Castle.bsp"},
    {"location",         "../../Fusion/Resources/Textures"},
};

std::map<std::string,unsigned int> GLMap =  {
    {"cullface",    GL_CULL_FACE},
    {"depth",       GL_DEPTH_TEST},
    {"blend",       GL_BLEND},
    {"multisample", GL_MULTISAMPLE}
};

std::variant<COMMAND_TYPE_VARIANCE> GetFromRegister(const std::string SearchKey)
{
    int Index = -1;
    for (int RegisterIndex = 0; RegisterIndex < RegisterList.size(); RegisterIndex++)
    {if (RegisterList[RegisterIndex].Key == SearchKey) {Index = RegisterIndex;}}
    return RegisterList[Index].Value;
}

void SetFromRegister(const std::string SetKey,std::variant<COMMAND_TYPE_VARIANCE> SetValue)
{
    bool AlreadyFound = false;
    for (int RegisterIndex = 0; RegisterIndex < RegisterList.size(); RegisterIndex++)
    {if (RegisterList[RegisterIndex].Key == SetKey) {RegisterList[RegisterIndex].Value = SetValue; AlreadyFound = true;}}
    if (!AlreadyFound) {CreateFromRegister(SetKey,SetValue); SysPrint("Cannot find "+SetKey+". Creating new register...",1);}
}

void CreateFromRegister(const std::string SetKey,std::variant<COMMAND_TYPE_VARIANCE> SetValue)
{
    Register NewRegister;
    NewRegister.Key     = SetKey;
    NewRegister.Value   = SetValue;
    RegisterList.push_back(NewRegister);
    SysPrint("Created new register: "+SetKey,1);
}

void CreateNewRegister(std::string Command)
{
    if (Command.find("+add") != std::string::npos)
    {
        std::istringstream  StringStream(Command);
        std::string         Command,Key;
        int                 Value;
        StringStream >> Command >> Key >> Value;
    
        if (std::holds_alternative<bool>(GetFromRegister(Key)))                 {CreateFromRegister(Key,((Value == 1) ? true : false));}
        else if (std::holds_alternative<int>(GetFromRegister(Key)))             {CreateFromRegister(Key,Value);} 
        else if (std::holds_alternative<float>(GetFromRegister(Key)))           {CreateFromRegister(Key,(float)Value);} 
        else if (std::holds_alternative<std::string>(GetFromRegister(Key)))     {CreateFromRegister(Key,std::to_string(Value));} 
        SysPrint("Successfully set "+Key+" to "+std::to_string(Value),1);
    }
}

void EditRegisterConsole(std::string Command)
{
    if (Command.find("+set") != std::string::npos)
    {
        std::istringstream  StringStream(Command);
        std::string         Command,Key;
        int                 Value;
        StringStream >> Command >> Key >> Value;
    
        if (std::holds_alternative<bool>(GetFromRegister(Key)))                 {SetFromRegister(Key,((Value == 1) ? true : false));}
        else if (std::holds_alternative<int>(GetFromRegister(Key)))             {SetFromRegister(Key,Value);} 
        else if (std::holds_alternative<float>(GetFromRegister(Key)))           {SetFromRegister(Key,(float)Value);} 
        else if (std::holds_alternative<std::string>(GetFromRegister(Key)))     {SetFromRegister(Key,std::to_string(Value));} 
        SysPrint("Successfully set "+Key+" to "+std::to_string(Value),1);
    }
}

void PlaySoundFusion(const std::string Command)
{
    if (Command.find("+play") != std::string::npos)
    {
        std::istringstream  StringStream(Command);
        std::string         Command,Key;
        StringStream >> Command >> Key;

        NewAudioController.AddSoundToLibrary(Key,{1.0f,1.0f,false,glm::vec3(0.0f),glm::vec3(0.0f),0,0});
        NewAudioController.Play(Key);
    }
}

void ClientTest(const std::string Command)
{
    NetworkClient Client;
    Client.StartClientNetwork("8000");
}

void ServerTest(const std::string Command)
{
    NetworkServer Server;
    Server.StartServerNetwork("8000");
}

void PrintAllRegisters(const std::string Command)
{
    for (int Index = 0; Index < RegisterList.size(); Index++)
    {
        if (std::holds_alternative<bool>(RegisterList[Index].Value))                {SysPrint(RegisterList[Index].Key+" = "+std::to_string(std::get<bool>(RegisterList[Index].Value))+" (bool)",1);}
        else if (std::holds_alternative<int>(RegisterList[Index].Value))            {SysPrint(RegisterList[Index].Key+" = "+std::to_string(std::get<int>(RegisterList[Index].Value))+" (int)",1);}
        else if (std::holds_alternative<float>(RegisterList[Index].Value))          {SysPrint(RegisterList[Index].Key+" = "+std::to_string(std::get<float>(RegisterList[Index].Value))+" (unsigned int)",1);}
        else if (std::holds_alternative<std::string>(RegisterList[Index].Value))    {SysPrint(RegisterList[Index].Key+" = "+std::get<std::string>(RegisterList[Index].Value)+" (string)",1);}
    }
}

void ExitGameFunction(const std::string Command)
{
    SysPrint("Preparing to exit Fusion...",1);
    SetFromRegister("exit",true);
}

void Clear(const std::string Command) {ConsoleItems = {};}

void Dump(const std::string Command)
{
    try
    {
        std::ofstream NewFile("ConsoleLog.txt");
        for (int Index = 0; Index < ConsoleItems.size(); Index++) {NewFile << ConsoleItems[Index].CommandContents << "\n";}
        NewFile.close();
        SysPrint("Successfully dumped logs to ConsoleLog.txt",1);
    }
    catch(const std::exception& Exception)
    {
        SysPrint("Failed to dump logs! Reason: "+(std::string)(Exception.what()),1);
    }
}

void SetVSync(const std::string Command)
{
    if (std::get<bool>(GetFromRegister("vsyncactive"))) 
    {SetFromRegister("vsyncactive",!std::get<bool>(GetFromRegister("vsyncactive")));}
    glfwSwapInterval((std::get<bool>(GetFromRegister("vsyncactive")) == true) ? 1 : 0);
}

void UpdateVariables(const std::string Command)
{
    SysPrint("Updating GLValues...",1);
    for (int RegisterIndex = 0; RegisterIndex < RegisterList.size(); RegisterIndex++)
    {
        if (GLMap.find(RegisterList[RegisterIndex].Key) != GLMap.end() && std::holds_alternative<int>(RegisterList[RegisterIndex].Value))
        {
            if (std::get<int>(RegisterList[RegisterIndex].Value) == 1)  {glEnable(GLMap.find(RegisterList[RegisterIndex].Key)->second); SysPrint("Enabled "+RegisterList[RegisterIndex].Key,1);}
            else                                                        {glDisable(GLMap.find(RegisterList[RegisterIndex].Key)->second); SysPrint("Disabled "+RegisterList[RegisterIndex].Key,1);}
        }        
    }
}