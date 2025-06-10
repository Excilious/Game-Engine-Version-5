#ifndef __CONSOLE_H__
#define __CONSOLE_H__
#define GLM_ENABLE_EXPERIMENTAL

#define CONSOLE_ERROR   glm::vec3(1.0f,0.0f,0.0f)
#define CONSOLE_WARN    glm::vec3(1.0f,1.0f,0.0f)
#define CONSOLE_INFO    glm::vec3(1.0f,1.0f,1.0f)
#define CONSOLE_CLIENT  glm::vec3(0.0f,0.0f,1.0f)
#define CONSOLE_SERVER  glm::vec3(0.0f,1.0f,0.0f)

#include <windows.h>
#include "../../Dependancies/imgui/imgui.h"
#include "../../Dependancies/imgui/imgui_impl_opengl3.h"
#include "../../Dependancies/imgui/imgui_impl_glfw.h"
#include "../../Dependancies/glad/glad.h"
#include "../../Dependancies/glm/glm.hpp"
#include "../../Dependancies/GLFW/glfw3.h"
#include "../../Dependancies/glm/gtx/string_cast.hpp"
#include "../../Dependancies/glm/gtc/type_ptr.hpp"
#include "Commands.h"

struct CommandTemplate
{
    glm::vec3       CommandColour;
    std::string     CommandContents;
};

template <typename T> void              SysPrintSafe(const T Text,const int ErrorCode);
template <typename T> void              SysPrintVector(const T Text,const int ErrorCode);
typedef void                            (*Function) (const std::string Command);
void                                    CommandAddCommand(const std::string NewCommandString,Function CommandFunction);
void                                    CommandUpdateCommandExecution(const std::string CommandString);
void                                    CommandRenderConsoleFrame(GLFWwindow* EngineWindow);
extern std::vector<CommandTemplate>     ConsoleItems;
extern void                             SysPrint(const std::string Text,const int ErrorCode);
extern void                             CommandShowExitMessage(void);
extern void                             CommandGenerateValues(void);
extern bool                             ConsoleOpen;
extern char                             ConsoleInputBuffer[128];

#endif