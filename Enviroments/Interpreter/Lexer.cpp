#include "Lexer.h"
#include "../Common/Console.h"

Lexer::Lexer(void) {}

void Lexer::CreateLexer(std::string Source) 
{
    this->Position  = 0;
    this->Source    = Source;
}

Token Lexer::NextToken(void)
{
    while (this->Position < this->Source.size() && isspace(this->Source[this->Position]))   {this->Position++;}
    if (this->Position >= this->Source.size())                                              {return {LexerTokens::END_OF_FILE,""};}
    
    char Character = this->Source[this->Position];
    if (isalpha(Character))
    {
        size_t Start = this->Position;
        while (this->Position < this->Source.size() && (isalnum(this->Source[this->Position]) || this->Source[this->Position] == '_')) {this->Position++;}
        std::string Word = this->Source.substr(Start,this->Position-Start);

        if (Word == "if")       return {LexerTokens::IF,Word};
        if (Word == "else")     return {LexerTokens::ELSE,Word};
        return {LexerTokens::IDENTIFER,Word};
    }

    if (isdigit(Character))
    {
        size_t Start = this->Position;
        while (this->Position < this->Source.size() && isdigit(this->Source[this->Position])) {this->Position++;}
        return {LexerTokens::NUMBER,this->Source.substr(Start,this->Position-Start)};
    }
    this->Position++;

    switch (Character)
    {
        case '{': return {LexerTokens::LEFT_BRACE,"{"};
        case '}': return {LexerTokens::RIGHT_BRACE,"}"};
        case '(': return {LexerTokens::LEFT_BRACKET,"("};
        case ')': return {LexerTokens::RIGHT_BRACKET,")"};
        case '+': return {LexerTokens::PLUS,"+"};
        case '-': return {LexerTokens::MINUS,"-"};
        case ';': return {LexerTokens::SEMICOLON,";"};
    }
    
    return {LexerTokens::TOKEN_NULL,std::string(1,Character)};
}

