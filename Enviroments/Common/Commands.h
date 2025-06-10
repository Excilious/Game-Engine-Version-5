#ifndef __COMMANDS_HEADER__
#define __COMMANDS_HEADER__

#define COMMAND_TYPE_VARIANCE int,float,bool,unsigned int,std::string

#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <variant>

#include "../../Dependancies/glad/glad.h"
#include "../../Dependancies/GLFW/glfw3.h"
#include "../../Dependancies/glm/glm.hpp"

typedef struct
{
    std::string                         Key;
    std::variant<COMMAND_TYPE_VARIANCE> Value;
} Register;

extern void                                     CreateNewRegister(std::string Command);
extern void                                     CreateFromRegister(const std::string SetKey,std::variant<COMMAND_TYPE_VARIANCE> SetValue);
extern void                                     ExitGameFunction(const std::string Command);
extern void                                     UpdateVariables(const std::string Command);
extern void                                     SetVSync(const std::string Command);
extern void                                     Dump(const std::string Command);
extern void                                     Clear(const std::string Command);
extern void                                     PlaySoundFusion(const std::string Command);
extern void                                     ClientTest(const std::string Command);
extern void                                     ServerTest(const std::string Command);
extern void                                     EditRegisterConsole(std::string Command);
extern void                                     PrintAllRegisters(const std::string Command);
extern std::vector<Register>                    RegisterList;
extern std::variant<COMMAND_TYPE_VARIANCE>      GetFromRegister(const std::string SearchKey);
extern void                                     SetFromRegister(const std::string SetKey,std::variant<COMMAND_TYPE_VARIANCE> SetValue);

#endif