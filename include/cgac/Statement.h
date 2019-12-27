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
    static StmtNodePtr ParseCompoundStatement(Parser& parser);
    static StmtNodePtr ParseExpressionStatement(Parser& parser);

}; // StatementParser

}