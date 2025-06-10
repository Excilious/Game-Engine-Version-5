#include "Console.h"

typedef struct CommandFunctions
{
    Function        ExecutableFunction;
    std::string     CommandString;
} Command;

ImVector<char*>                 ConsoleRenderedItems;
std::vector<CommandTemplate>    ConsoleItems;
std::vector<Command>            ConsoleLists;
std::vector<std::string>        LastCommands        = {};
bool                            ConsoleOpen         = false;
bool                            ConsoleToBottom     = false;
bool                            ConsoleReclaimFocus = false;
int                             LastCommandIndex    = 0;
int                             LastCommandScroll   = 0;
char                            ConsoleInputBuffer[128];

template <typename T>
void SysPrintSafe(const T Text,const int ErrorCode) {SysPrint(std::to_string(Text),ErrorCode);}

template <typename T>
void SysPrintVector(const T Text,const int ErrorCode) {SysPrint(glm::to_string(Text),ErrorCode);}

static char* Strdup(const char* s)                           
{IM_ASSERT(s); size_t len = strlen(s) + 1; void* buf = ImGui::MemAlloc(len); IM_ASSERT(buf); return (char*)memcpy(buf,(const void*)s,len);}

void CommandAddRendered(const char* Format, ...)
{
    char Buffer[1024];
    va_list Arguments;
    va_start(Arguments,Format);
    vsnprintf(Buffer,IM_ARRAYSIZE(Buffer),Format,Arguments);
    Buffer[IM_ARRAYSIZE(Buffer)-1] = 0;
    va_end(Arguments);
    ConsoleRenderedItems.push_back(Strdup(Buffer));
}

void SysPrint(const std::string Text,const int ErrorCode = 1)
{
    glm::vec3       Colour;
    CommandTemplate NewCommandTemplate;
    CommandAddRendered(Text.c_str());

    if      (ErrorCode == 1)    {Colour = CONSOLE_INFO;}
    else if (ErrorCode == 2)    {Colour = CONSOLE_WARN;}
    else if (ErrorCode == 3)    {Colour = CONSOLE_ERROR;}
    else if (ErrorCode == 4)    {Colour = CONSOLE_CLIENT;}
    else if (ErrorCode == 5)    {Colour = CONSOLE_SERVER;}
    NewCommandTemplate.CommandContents  = Text;
    NewCommandTemplate.CommandColour    = Colour;
    ConsoleItems.push_back(NewCommandTemplate);
}

void CommandAddCommand(const std::string NewCommandString,Function CommandFunction)
{
    Command NewCommand;
    for (int CommandIndex = 0; CommandIndex < ConsoleLists.size(); CommandIndex++)
    {if (ConsoleLists[CommandIndex].CommandString == NewCommandString) {SysPrint("Command already exists.",2); return;}}
    NewCommand.CommandString        = NewCommandString;
    NewCommand.ExecutableFunction   = CommandFunction;
    ConsoleLists.push_back(NewCommand);
}

void CommandUpdateCommandExecution(const std::string CommandString)
{
    int Index = -1;
    for (int NewIndex = 0; NewIndex < ConsoleLists.size(); NewIndex++)
    {
        if (ConsoleLists[NewIndex].CommandString == CommandString) {Index=NewIndex;}
    }
    if (Index != -1)                                            {ConsoleLists[Index].ExecutableFunction(CommandString);}
    else if (CommandString.find("+set") != std::string::npos)   {EditRegisterConsole(CommandString);}
    else if (CommandString.find("+add") != std::string::npos)   {CreateNewRegister(CommandString);}
    else if (CommandString.find("+play") != std::string::npos)  {PlaySoundFusion(CommandString);}
    else                                                        {SysPrint("Invalid/Unknown Command",3);}
    LastCommands.push_back(CommandString);
}

void CommandRenderConsoleFrame(GLFWwindow* EngineWindow)
{
    if (!ConsoleOpen) {return;}
    ImGui::Begin("Console",&ConsoleOpen);
    ImGui::Separator();
    ConsoleReclaimFocus     = std::get<bool>(GetFromRegister("consolefocuswhenactive"));

    if (glfwGetKey(EngineWindow,GLFW_KEY_UP) == GLFW_PRESS && ConsoleOpen && LastCommands.size() > 0 && glfwGetTime()-LastCommandScroll >= 2)
    {
        LastCommandScroll = glfwGetTime();
        SysPrint(LastCommands[LastCommandIndex],2);
        SysPrint(std::to_string(LastCommandIndex),2);
        strncat(ConsoleInputBuffer,LastCommands[LastCommandIndex].c_str(),sizeof(ConsoleInputBuffer)-strlen(ConsoleInputBuffer)-1);
        if (LastCommandIndex < LastCommands.size()) {LastCommandIndex++;}
    }
    if (glfwGetKey(EngineWindow,GLFW_KEY_DOWN) == GLFW_PRESS && ConsoleOpen && LastCommands.size() > 0 && glfwGetTime()-LastCommandScroll >= 2)
    {
        LastCommandScroll = glfwGetTime();
        SysPrint(LastCommands[LastCommandIndex],2);
        SysPrint(std::to_string(LastCommandIndex),2);
        if (LastCommandIndex > 0) {LastCommandIndex--;}
    }

    const float HeightToReserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    if (ImGui::BeginChild("ScrollingRegion",ImVec2(0,-HeightToReserve),ImGuiChildFlags_None,ImGuiWindowFlags_HorizontalScrollbar))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4,1));
        for (int Index = 0; Index < ConsoleRenderedItems.size(); Index++)
        {
            ImVec4  Colour;
            bool    HasColour = false;
            if (ConsoleItems[Index].CommandColour == CONSOLE_INFO)              {Colour = ImVec4(1.0f,1.0f,1.0f,1.0f); HasColour = true;}
            else if (ConsoleItems[Index].CommandColour == CONSOLE_WARN)         {Colour = ImVec4(1.0f,1.0f,0.4f,1.0f); HasColour = true;}
            else if (ConsoleItems[Index].CommandColour == CONSOLE_ERROR)        {Colour = ImVec4(1.0f,0.4f,0.4f,1.0f); HasColour = true;}
            else if (ConsoleItems[Index].CommandColour == CONSOLE_SERVER)       {Colour = ImVec4(0.4f,1.0f,0.4f,1.0f); HasColour = true;}
            else if (ConsoleItems[Index].CommandColour == CONSOLE_CLIENT)       {Colour = ImVec4(0.4f,0.4f,1.0f,1.0f); HasColour = true;}
            if (HasColour) {ImGui::PushStyleColor(ImGuiCol_Text,Colour);}
            ImGui::TextUnformatted(ConsoleRenderedItems[Index]);
            if (HasColour) {ImGui::PopStyleColor();}
        }
        if (ConsoleToBottom || (std::get<bool>(GetFromRegister("consoleactivescroll")) && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())) 
        {ImGui::SetScrollHereY(1.0f);}
        ConsoleToBottom = false;

        ImGui::PopStyleVar();
    }
    ImGui::EndChild();
    ImGui::Separator();

    ImGuiInputTextFlags TextFlags = ImGuiInputTextFlags_EnterReturnsTrue;
    if (ImGui::InputText("Input",ConsoleInputBuffer,IM_ARRAYSIZE(ConsoleInputBuffer),TextFlags))
    {
        SysPrint(ConsoleInputBuffer,1);
        if (std::string(ConsoleInputBuffer).find("+exec") == std::string::npos) 
        {
            CommandUpdateCommandExecution(std::string(ConsoleInputBuffer));
            ConsoleInputBuffer[0]   = '\0';
            ConsoleReclaimFocus     = true;
        }
    }

    ImGui::SetItemDefaultFocus();
    if (ConsoleReclaimFocus) {ImGui::SetKeyboardFocusHere(-1);}
    ImGui::End();
}

void CommandShowExitMessage(void)
{
    if (std::get<bool>(GetFromRegister("exit")))
    {
        ImGui::OpenPopup("Quit?");
        ImVec2 Center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(Center,ImGuiCond_Appearing,ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal("Quit?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Are you sure you want to leave?\nPress 'OK' to continue.");
            ImGui::Separator();

            if (ImGui::Button("OK", ImVec2(120, 0))) {SetFromRegister("exitconfirm",true); SetFromRegister("exit",false);}
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();  
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {SetFromRegister("exit",false); ImGui::CloseCurrentPopup();}
            ImGui::EndPopup();
        }
    }
}

void CommandGenerateValues(void)
{
    SetFromRegister("width",1920.0f);
    SetFromRegister("height",1080.0f);
    SetFromRegister("showhud",true);
    SetFromRegister("consolefocuswhenactive",false);
    SetFromRegister("consoleactivescroll",true);
    SetFromRegister("gravity",9.81f);
    SetFromRegister("accel",5.0f);
    SetFromRegister("decel",0.0005f);
    SetFromRegister("speed",35.0f);
    SetFromRegister("frict",0.006f);
    SetFromRegister("mass",2.0f);

    CommandAddCommand("exit",ExitGameFunction);
    CommandAddCommand("-lstreg",PrintAllRegisters);
    CommandAddCommand("-update",UpdateVariables);
    CommandAddCommand("-dump",Dump);
    CommandAddCommand("-clear",Clear);
    CommandAddCommand("+netc",ClientTest);
    CommandAddCommand("+nets",ServerTest);
    //CommandAddCommand("-vsync",SetVSync);
}
