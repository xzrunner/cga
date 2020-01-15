#pragma once

#include "cgac/AST.h"
#include "cgac/typedef.h"

namespace cgac
{

struct StatementNode : public Node
{
    StatementNode(const Tokenizer& lexer, NodeKind kind)
        : Node(lexer, kind) {}

}; // StatementNode

struct ExprStmtNode : public StatementNode
{
    ExprStmtNode(const Tokenizer& lexer, NodeKind kind)
        : StatementNode(lexer, kind) {}

    ExprNodePtr expr = nullptr;

}; // ExprStmtNode

struct CompoundStmtNode : public StatementNode
{
    CompoundStmtNode(const Tokenizer& lexer, NodeKind kind)
        : StatementNode(lexer, kind) {}

    NodePtr stmts = nullptr;

}; // CompoundStmtNode

struct CaseStmtNode : public StatementNode
{
    CaseStmtNode(const Tokenizer& lexer, NodeKind kind)
        : StatementNode(lexer, kind) {}

    ExprNodePtr expr = nullptr;

    NodePtr stmt = nullptr;

}; // CaseStmtNode

struct ElseStmtNode : public StatementNode
{
    ElseStmtNode(const Tokenizer& lexer, NodeKind kind)
        : StatementNode(lexer, kind) {}

    NodePtr stmt = nullptr;

}; // ElseStmtNode

struct RuleStmtNode : public StatementNode
{
    RuleStmtNode(const Tokenizer& lexer, NodeKind kind)
        : StatementNode(lexer, kind) {}

    const char* name = nullptr;
    ExprNodePtr params = nullptr;

    NodePtr stmts = nullptr;

}; // RuleStmtNode

struct SelectorStmtNode : public StatementNode
{
    SelectorStmtNode(const Tokenizer& lexer, NodeKind kind)
        : StatementNode(lexer, kind) {}

    ExprNodePtr expr = nullptr;

    bool repeat = false;

}; // SelectorStmtNode

class Parser;

class StatementParser
{
public:
    static StmtNodePtr ParseStatement(Parser& parser);

private:
    static StmtNodePtr ParseExpressionStatement(Parser& parser);

}; // StatementParser

}