#include "VirtualMachine.h"
#include "../Common/Console.h"

VirtualMachine::VirtualMachine(void) {}

void VirtualMachine::Execute(std::string NewScript,Parser* EngineParser)
{
    if (NewScript.find(".fex") == std::string::npos && NewScript.find(".") == std::string::npos) {NewScript+=".fex";}
    
    this->CurrentTime = glfwGetTime();
    std::string ScriptContents = EngineParser->GetGlobalDatapackData(NewScript);
    if (ScriptContents != "")
    {
        this->EngineLexer.CreateLexer(ScriptContents);
        this->EngineScriptParser.CreateParser(this->EngineLexer);

        try
        {
            while (true)
            {
                auto Statement = this->EngineScriptParser.ParseStatement();
                if (!Statement) {break;}
                SysPrint("Parsed Statement!",2);
            }
        }
        catch(const std::exception& e)
        {
            //SysPrint(e,2);
        }
        
    }
    else
    {SysPrint("Cannot open executable file "+NewScript,2); return;}
    SysPrint("Completed executing "+NewScript+" at "+std::to_string(glfwGetTime()-this->CurrentTime),1);
}