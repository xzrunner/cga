#include "cgac/DumpAST.h"

#include <stdarg.h>

namespace
{

void LeftAlign(std::ostream& output, int pos)
{
	char spaces[256];

	pos = pos >= 256 ? 2 : pos;
	memset(spaces, ' ', pos);
	spaces[pos] = '\0';
    output << "\n" << spaces;
}

std::string StringFormat(const std::string fmt, ...)
{
	int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
	std::string str;
	va_list ap;
	while (1) {     // Maximum two passes on a POSIX system...
		str.resize(size);
		va_start(ap, fmt);
		int n = vsnprintf((char *)str.data(), size, fmt.c_str(), ap);
		va_end(ap);
		if (n > -1 && n < size) {  // Everything worked
			str.resize(n);
			return str;
		}
		if (n > -1)  // Needed size returned
			size = n + 1;   // For null char
		else
			size *= 2;      // Guess at a larger size (OS specific)
	}
	return str;
}

}

namespace cgac
{

void DumpExpression(std::ostream& output, const ExprNodePtr& expr, int pos)
{
	if (expr == NULL)
	{
		output << "NIL";
		return;
	}

    char *opname = NULL;
	if (expr->op >= OP_COMMA && expr->op < OP_ID) {
		opname = ExpressionParser::OPNames[expr->op];
	}

	switch (expr->op)
	{
	case OP_COMMA:
	case OP_ASSIGN:
    case OP_COLON:
	case OP_OR:
	case OP_AND:
    case OP_SEPARATOR:
	case OP_EQUAL:
	case OP_UNEQUAL:
	case OP_GREAT:
	case OP_LESS:
	case OP_GREAT_EQ:
	case OP_LESS_EQ:
	case OP_ADD:
	case OP_SUB:
	case OP_MUL:
	case OP_DIV:
	case OP_MOD:
    case OP_RULE:
    case OP_NEG:
    case OP_NOT:
	case OP_INDEX:
        output << "(" << opname << " ";
		pos += strlen(opname) + 2;
		DumpExpression(output, expr->kids[0], pos);
		LeftAlign(output, pos);
		DumpExpression(output, expr->kids[1], pos);
        output << ")";
		break;

    case OP_COMP:
        output << "(" << opname << " ";
        pos += strlen(opname) + 2;
        DumpExpression(output, expr->kids[0], pos);
        output << ")";
        break;

	case OP_CALL:
    {
        ExprNodePtr p = expr->kids[1];

        output << "(" << opname << " ";
        pos += strlen(opname) + 2;
        DumpExpression(output, expr->kids[0], pos);
        while (p)
        {
            LeftAlign(output, pos);
            DumpExpression(output, p, pos);
            if (p->next != nullptr) {
                output << ",";
            }
            p = std::static_pointer_cast<ExpressionNode>(p->next);
        }
        output << ")";
    }
		break;

	case OP_MEMBER:
		output << "(" << opname << " ";
		pos += strlen(opname) + 2;
		DumpExpression(output, expr->kids[0], pos);
		LeftAlign(output, pos);
        output << (char*)(expr->val.p);
		break;

    case OP_DUPLICATE:
        assert(0);
        break;

	case OP_ID:
        output << (char*)(expr->val.p);
		break;

	case OP_CONST:
	{
		int categ = expr->ty->categ;
        std::string s;
		if (categ == FLOAT) {
            s = StringFormat("%g", expr->val.f);
		} else {
            assert(0);
        }
        output << s;
	}
	break;

	case OP_STR:
	{
		output << "\"";

        auto str = (char*)(expr->val.p);
		for (int i = 0, n = strlen(str); i < n; ++i)
		{
            std::string s;
            if (isprint(str[i])) {
                s = StringFormat("%c", str[i]);
            } else {
                s = StringFormat("\\x%x", str[i]);
            }
            output << s;
		}

        output << "\"";
	}
	break;

	default:
        output << "ERR";
		break;
	}
}

void DumpStatement(std::ostream& output, const StmtNodePtr& stmt, int pos)
{
    switch (stmt->kind)
    {
    case NK_SymbolStatement:
        DumpExpression(output, std::static_pointer_cast<SymbolStmtNode>(stmt)->expr, pos);
        break;

    case NK_ExpressionStatement:
        DumpExpression(output, std::static_pointer_cast<ExprStmtNode>(stmt)->expr, pos);
        break;

    case NK_CompoundStatement:
    {
        auto p = std::static_pointer_cast<CompoundStmtNode>(stmt)->stmts;

        output << "{";
        while (p != NULL)
        {
            LeftAlign(output, pos + 2);
            DumpStatement(output, std::static_pointer_cast<StatementNode>(p), pos + 2);
            if (p->next != NULL) {
                output << "\n";
            }
            p = p->next;
        }
        LeftAlign(output, pos);
        output << "}";
    }
        break;

    case NK_RuleStatement:
    {
        auto rule_stmt = std::static_pointer_cast<RuleStmtNode>(stmt);

        output << "(rule " << rule_stmt->rule << ":\n";
        LeftAlign(output, pos + 2);
        DumpStatement(output, rule_stmt->stmt, pos + 2);
        LeftAlign(output, pos);
        output << "end-rule)";
    }
        break;

    }
}

}