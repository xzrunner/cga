#pragma once

#include <memory>

namespace cgac
{

class Tokenizer;

enum NodeKind
{
    NK_Expression,

    NK_ExpressionStatement, NK_CompoundStatement,

    NK_MaxCount,
};

struct Coord
{
    Coord(const Tokenizer& lexer);

    size_t line   = 0;
    size_t column = 0;
};

struct Node
{
    Node(const Tokenizer& lexer, NodeKind kind);
    Node(const Coord& coord, NodeKind kind);

    NodeKind kind = NK_MaxCount;

    Coord coord;

    std::shared_ptr<Node> next = nullptr;

}; // Node

using NodePtr = std::shared_ptr<Node>;

class ASTHelper
{
public:
    static bool CurrentTokenIn(int curr_token, int toks[]);
    static void PostCheckTypedef();

}; // ASTHelper

}