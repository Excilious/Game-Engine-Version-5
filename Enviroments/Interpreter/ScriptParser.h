#ifndef __SCRIPT_PARSER_HEADER__
#define __SCRIPT_PARSER_HEADER__

#include <vector>
#include <memory>
#include "Lexer.h"

struct Expression
{virtual ~Expression() = default;};

struct Statement
{virtual ~Statement() = default;};

struct BinaryExpression: Expression
{
    std::unique_ptr<Expression>     Left;
    std::string                     Opcode;
    std::unique_ptr<Expression>     Right;
    BinaryExpression(std::unique_ptr<Expression> PLeft,std::string Code,std::unique_ptr<Expression> PRight) 
    : Left(std::move(PLeft)), Opcode(Code), Right(std::move(PRight)) {}
};

struct LiteralExpression: Expression
{
    int Value;
    LiteralExpression(int NewValue): Value(NewValue) {}
};

struct VariableExpression: Expression
{
    std::string Name;
    VariableExpression(const std::string& NewName): Name(NewName) {}
};

struct ExpressionStatement: Statement
{
    std::unique_ptr<Expression> ExpressionObject;
    ExpressionStatement(std::unique_ptr<Expression> NewExpression): ExpressionObject(std::move(NewExpression)) {}
};

struct BlockStatement: Statement
{std::vector<std::unique_ptr<Statement>> Statements;};

struct IfStatement: Statement {
    std::unique_ptr<Expression> Condition;
    std::unique_ptr<Statement>  ThenBranch;
    std::unique_ptr<Statement>  ElseBranch;

    IfStatement(std::unique_ptr<Expression> cond,
                std::unique_ptr<Statement> thenBranch,
                std::unique_ptr<Statement> elseBranch)
        : Condition(std::move(cond)),
          ThenBranch(std::move(thenBranch)),
          ElseBranch(std::move(elseBranch)) {}
};


class ScriptParser
{
    public:
                                                ScriptParser(void);
        void                                    CreateParser(Lexer& NewLexer);
        void                                    Advance(void);
        void                                    Expect(LexerTokens NewType,const std::string& ErrorMessage);
        bool                                    Match(LexerTokens NewType);
        std::unique_ptr<Statement>              ParseStatement(void);
        std::unique_ptr<BlockStatement>         ParseBlock(void);
        std::unique_ptr<IfStatement>            ParseIfStatement(void);
        std::unique_ptr<ExpressionStatement>    ParseExpressionStatement(void);
        std::unique_ptr<Expression>             ParseExpression(void);
        std::unique_ptr<Expression>             ParseTerm(void);
        std::unique_ptr<Expression>             ParseFactor(void);
    private:
        Token                                   CurrentToken;
        Lexer*                                  CurrentLexer = nullptr;
};

#endif