#include "ScriptParser.h"
#include "../Common/Console.h"

ScriptParser::ScriptParser(void) {}

void ScriptParser::CreateParser(Lexer& NewLexer) {this->CurrentLexer = &NewLexer;};

void ScriptParser::Advance(void) {this->CurrentToken = this->CurrentLexer->NextToken();}
void ScriptParser::Expect(LexerTokens NewType,const std::string& ErrorMessage)
{
    if (this->CurrentToken.CurrentType != NewType) {SysPrint(ErrorMessage,3);}
    this->Advance();
}

bool ScriptParser::Match(LexerTokens NewType)
{
    if (this->CurrentToken.CurrentType == NewType)
    {
        this->Advance();
        return true;
    }
    return false;
}

std::unique_ptr<Statement> ScriptParser::ParseStatement(void)
{
    if (this->Match(LexerTokens::IF))             {return this->ParseIfStatement();}
    if (this->Match(LexerTokens::LEFT_BRACE))     {return this->ParseBlock();}
    return this->ParseExpressionStatement();
}

std::unique_ptr<BlockStatement> ScriptParser::ParseBlock(void)
{
    auto Block = std::make_unique<BlockStatement>();
    while (!this->Match(LexerTokens::RIGHT_BRACE))
    {
        if (this->CurrentToken.CurrentType == LexerTokens::END_OF_FILE) {SysPrint("Unterminated block",3);}
        Block->Statements.push_back(this->ParseStatement());
    }
    return Block;
}

std::unique_ptr<IfStatement> ScriptParser::ParseIfStatement(void)
{
    this->Expect(LexerTokens::LEFT_BRACE, "Expected identifier '(' after 'if'.");
    auto Condition = this->ParseExpression();
    this->Expect(LexerTokens::RIGHT_BRACE, "Expected identifier ')' after 'if' condition.");
    auto ThenBranch = this->ParseStatement();

    std::unique_ptr<Statement> ElseBranch = nullptr;
    if (this->Match(LexerTokens::ELSE)) {
        ElseBranch = this->ParseStatement();
    }
    return std::make_unique<IfStatement>(
        std::move(Condition),
        std::move(ThenBranch),
        std::move(ElseBranch)
    );
}

std::unique_ptr<ExpressionStatement> ScriptParser::ParseExpressionStatement(void)
{
    auto Expression = this->ParseExpression();
    this->Expect(LexerTokens::SEMICOLON,"Expected ';' after expression");
    return std::make_unique<ExpressionStatement>(ExpressionStatement{std::move(Expression)});
}

std::unique_ptr<Expression> ScriptParser::ParseExpression(void) {return this->ParseTerm();}
std::unique_ptr<Expression> ScriptParser::ParseTerm(void)
{
    auto Expression = this->ParseFactor();
    while (this->CurrentToken.CurrentType == LexerTokens::PLUS || this->CurrentToken.CurrentType == LexerTokens::MINUS)
    {
        std::string Opcode = this->CurrentToken.Label;
        this->Advance();
        auto Right = this->ParseFactor();
        Expression = std::make_unique<BinaryExpression>(std::move(Expression),Opcode,std::move(Right));
    }
    return Expression;
}

std::unique_ptr<Expression> ScriptParser::ParseFactor(void)
{
    if (this->Match(LexerTokens::NUMBER))
    {
        int Value = std::stoi(this->CurrentToken.Label);
        return std::make_unique<LiteralExpression>(Value);
    }
    if (this->Match(LexerTokens::IDENTIFER))      {return std::make_unique<VariableExpression>(this->CurrentToken.Label);}
    if (this->Match(LexerTokens::LEFT_BRACKET))
    {
        auto Expression = this->ParseExpression();
        this->Expect(LexerTokens::RIGHT_BRACKET,"Expected ')' after expression");
        return Expression;
    }
    SysPrint("Unexpected token in expression",3);
    return nullptr;
}