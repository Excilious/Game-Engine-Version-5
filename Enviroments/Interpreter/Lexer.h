#ifndef __LEXER_HEADER__
#define __LEXER_HEADER__

#include <map>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

enum class LexerTokens
{
    IF,ELSE,
    LEFT_BRACE,RIGHT_BRACE,
    LEFT_BRACKET,RIGHT_BRACKET,
    IDENTIFER,NUMBER,
    PLUS,MINUS,
    SEMICOLON,
    END_OF_FILE,
    TOKEN_NULL,
};

struct Token
{
    LexerTokens     CurrentType;
    std::string     Label;
};

class Lexer
{
    public:
                        Lexer(void);
        void            CreateLexer(std::string Source);
        Token           NextToken(void);
    private:
        std::string     Source;
        size_t          Position;
};



#endif