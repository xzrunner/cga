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

struct SymbolStmtNode : public StatementNode
{
    SymbolStmtNode(const Tokenizer& lexer, NodeKind kind)
        : StatementNode(lexer, kind) {}

    ExprNodePtr expr = nullptr;

}; // SymbolStmtNode

struct ExprStmtNode : public StatementNode
{
    ExprStmtNode(const Tokenizer& lexer, NodeKind kind)
        : StatementNode(lexer, kind) {}

    ExprNodePtr expr = nullptr;

}; // ExprStmtNode

struct RuleStmtNode : public StatementNode
{
    RuleStmtNode(const Tokenizer& lexer, NodeKind kind)
        : StatementNode(lexer, kind) {}

    char* rule = nullptr;
    StmtNodePtr stmt = nullptr;

}; // RuleStmtNode

struct SelectorStmtNode : public StatementNode
{
    SelectorStmtNode(const Tokenizer& lexer, NodeKind kind)
        : StatementNode(lexer, kind) {}

    char* selector = nullptr;
    StmtNodePtr stmt = nullptr;

}; // SelectorStmtNode

struct CompoundStmtNode : public StatementNode
{
    CompoundStmtNode(const Tokenizer& lexer, NodeKind kind)
        : StatementNode(lexer, kind) {}

    //NodePtr decls = nullptr;
    NodePtr stmts = nullptr;
//    Vector ilocals;

}; // CompoundStmtNode

class Parser;

class StatementParser
{
public:
    static StmtNodePtr ParseStatement(Parser& parser);

private:
    static StmtNodePtr ParseSymbolStatement(Parser& parser);
    static StmtNodePtr ParseCompoundStatement(Parser& parser);
    static StmtNodePtr ParseExpressionStatement(Parser& parser);
    static StmtNodePtr ParseRuleStatement(Parser& parser);

}; // StatementParser

}