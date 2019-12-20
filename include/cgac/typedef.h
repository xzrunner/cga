#pragma once

#include <memory>

namespace cgac
{

struct ExpressionNode;
using ExprNodePtr = std::shared_ptr<ExpressionNode>;

struct StatementNode;
using StmtNodePtr = std::shared_ptr<StatementNode>;

}