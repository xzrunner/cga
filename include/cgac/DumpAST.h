#pragma once

#include "cgac/Expression.h"
#include "cgac/Statement.h"

namespace cgac
{

void DumpExpression(std::ostream& output, const ExprNodePtr& expr, int pos);
void DumpStatement(std::ostream& output, const StmtNodePtr& stmt, int pos);

}