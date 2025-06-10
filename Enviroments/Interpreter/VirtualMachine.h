#ifndef __VIRTUAL_MACHINE_HEADER__
#define __VIRTUAL_MACHINE_HEADER__

#include <string>
#include <vector>
#include "../Common/Parser.h"
#include "Lexer.h"
#include "ScriptParser.h"

class VirtualMachine
{
    public:
                                    VirtualMachine(void);
        void                        Execute(std::string NewScript,Parser* EngineParser);
    private:
        Lexer                       EngineLexer;
        ScriptParser                EngineScriptParser;
        std::vector<std::string>    CachedScript;
        float                       CurrentTime;
};

#endif