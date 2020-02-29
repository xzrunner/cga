#pragma once

#include <memory>

namespace cga
{

struct ExpressionNode;
using ExprNodePtr = std::shared_ptr<ExpressionNode>;

struct StatementNode;
using StmtNodePtr = std::shared_ptr<StatementNode>;

}