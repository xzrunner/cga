#include "cgac/Statement.h"
#include "cgac/Parser.h"
#include "cgac/grammer.h"
#include "cgac/Expression.h"

namespace cgac
{

static int FIRST_Statement[] = { FIRST_STATEMENT, 0};

StmtNodePtr StatementParser::ParseStatement(Parser& parser)
{
    switch (parser.CurrTokenType())
    {
    case TK_LBRACE:
        return ParseCompoundStatement(parser);

    default:
        return ParseExpressionStatement(parser);
    }
}

/**
 *  compound-statement:
 *		{ [statement-list] }
 *  statement-list:
 *		statement
 *		statement-list statement
 */
StmtNodePtr StatementParser::ParseCompoundStatement(Parser& parser)
{
//	Level++;

    auto comp_stmt = std::make_shared<CompoundStmtNode>(parser.GetTokenizer(), NK_CompoundStatement);

	parser.NextToken();
    NodePtr* tail = &comp_stmt->stmts;
	while (parser.CurrTokenType() != TK_RBRACE && parser.CurrTokenType() != TK_END)
	{
		*tail = ParseStatement(parser);
		tail = &(*tail)->next;
        if (parser.CurrTokenType() == TK_RBRACE) {
            break;
        }
		parser.SkipTo(FIRST_Statement);
	}
	parser.Expect(TK_RBRACE);
    parser.NextToken();

    ASTHelper::PostCheckTypedef();
//	Level--;

	return comp_stmt;
}

/**
 *  expression-statement:
 *		[expression] ;
 */
StmtNodePtr StatementParser::ParseExpressionStatement(Parser& parser)
{
    auto expr_stmt = std::make_shared<ExprStmtNode>(parser.GetTokenizer(), NK_ExpressionStatement);
    expr_stmt->expr = ExpressionParser::ParseExpression(parser);
	return expr_stmt;
}

}