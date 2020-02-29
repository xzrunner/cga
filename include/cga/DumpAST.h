#pragma once

#include "cga/Expression.h"
#include "cga/Statement.h"

namespace cga
{

void DumpExpression(std::ostream& output, const ExprNodePtr& expr, int pos);
void DumpStatement(std::ostream& output, const StmtNodePtr& stmt, int pos);

}