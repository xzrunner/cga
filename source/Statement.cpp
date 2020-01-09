#include "cgac/Statement.h"
#include "cgac/Parser.h"
#include "cgac/grammer.h"
#include "cgac/Expression.h"

namespace cgac
{

StmtNodePtr StatementParser::ParseStatement(Parser& parser)
{
    return ParseCompoundStatement(parser);
}

/**
 *  compound-statement:
 *		[statement-list]
 *  statement-list:
 *		statement
 *		statement-list statement
 */
StmtNodePtr StatementParser::ParseCompoundStatement(Parser& parser)
{
    auto comp_stmt = std::make_shared<CompoundStmtNode>(parser.GetTokenizer(), NK_CompoundStatement);

    NodePtr* tail = &comp_stmt->stmts;
    while (parser.CurrTokenType() != TK_END)
    {
        *tail = ParseExpressionStatement(parser);
        tail = &(*tail)->next;
    }
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