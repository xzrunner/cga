#pragma once

#include "cga/AST.h"
#include "cga/Type.h"
#include "cga/Tokenizer.h"
#include "cga/typedef.h"

namespace cga
{

enum OP
{
#define OPINFO(op, prec, name, func, opcode) op,
#include "cga/op_cfg.h"
#undef OPINFO
};

class Parser;

struct ExpressionNode : public Node
{
    ExpressionNode(const Tokenizer& lexer, NodeKind kind);

    std::unique_ptr<Type> ty = nullptr;
    int op      : 16;
    int isarray : 1;
    int isfunc  : 1;
    int lvalue  : 1;
    int bitfld  : 1;
    int inreg   : 1;
    int unused  : 11;
    std::array<ExprNodePtr, 2> kids;
    union Value val;

}; // ExpressionNode

class ExpressionParser
{
public:
    static ExprNodePtr ParseExpression(Parser& parser);
    static ExprNodePtr ParseAssignmentExpression(Parser& parser);
    static ExprNodePtr ParsePrimaryExpression(Parser& parser);

private:
    static ExprNodePtr ParsePostfixExpression(Parser& parser);
    static ExprNodePtr ParseUnaryExpression(Parser& parser);
    static ExprNodePtr ParseBinaryExpression(Parser& parser, int prec);

public:
    static char* OPNames[];

}; // ExpressionParser

}