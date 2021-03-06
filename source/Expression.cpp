#include "cga/Expression.h"
#include "cga/Parser.h"

namespace
{

using namespace cga;

struct TokenOp
{
	int bop  : 16;
	int uop  : 16;
};

struct TokenOp TokenOps[] =
{
#define TOKENOP(tok, bop, uop) {bop, uop},
#include "cga/token_op_cfg.h"
#undef  TOKENOP
};

#define IsBinaryOP(tok) (tok >= TK_OR && tok <= TK_MOD)
#define	BINARY_OP(tok)   TokenOps[tok - TK_ASSIGN].bop
#define UNARY_OP(tok)    TokenOps[tok - TK_ASSIGN].uop

int Prec[] =
{
#define OPINFO(op, prec, name, func, opcode) prec,
#include "cga/op_cfg.h"
	0
#undef OPINFO
};

}

namespace cga
{

char* ExpressionParser::OPNames[] =
{
#define OPINFO(op, prec, name, func, opcode) name,
#include "cga/op_cfg.h"
	NULL
#undef OPINFO
};

ExpressionNode::ExpressionNode(const Tokenizer& lexer, NodeKind kind)
    : Node(lexer, kind)
{
}

/**
 *  expression:
 *      assignment-expression
 *      expression , assignment-expression
 */
ExprNodePtr ExpressionParser::ParseExpression(Parser& parser)
{
    auto expr = ParseAssignmentExpression(parser);
    while (parser.CurrTokenType() == TK_COMMA)
    {
        auto coma_expr = std::make_shared<ExpressionNode>(parser.GetTokenizer(), NK_Expression);
        coma_expr->op = OP_COMMA;
        coma_expr->kids[0] = expr;
        parser.NextToken();
        coma_expr->kids[1] = ParseAssignmentExpression(parser);

        expr = coma_expr;
    }

    return expr;
}

/**
 *  assignment-expression:
 *      conditional-expression
 *      unary-expression assignment-operator assignment-expression
 *  assignment-operator:
 *      = *= /= %= += -= <<= >>= &= ^= |=
 *  There is a little twist here: the parser always treats the first nonterminal
 *  as a conditional expression.
 */
ExprNodePtr ExpressionParser::ParseAssignmentExpression(Parser& parser)
{
	auto expr = ParseBinaryExpression(parser, Prec[OP_OR]);
    auto curr_token = parser.CurrTokenType();
	if (curr_token == TK_ASSIGN)
	{
        auto asgn_expr = std::make_shared<ExpressionNode>(parser.GetTokenizer(), NK_Expression);
		asgn_expr->op = BINARY_OP(curr_token);
		asgn_expr->kids[0] = expr;
        parser.NextToken();
		asgn_expr->kids[1] = ParseAssignmentExpression(parser);

		return asgn_expr;
	}

	return expr;
}


/**
 *  primary-expression:
 *		ID
 *		constant
 *		string-literal
 *		( expression )
 *		{ expression }
 */
ExprNodePtr ExpressionParser::ParsePrimaryExpression(Parser& parser)
{
	switch (parser.CurrTokenType())
	{
    case TK_ATTR:
    {
        auto expr = std::make_shared<ExpressionNode>(parser.GetTokenizer(), NK_Expression);
        expr->ty = std::make_unique<Type>(Types[ATTR]);
        expr->op = OP_ATTR;
        expr->val = parser.GetTokenizer().GetTokenVal();
        parser.NextToken();

        return expr;
    }

    case TK_CONST:
    {
        auto expr = std::make_shared<ExpressionNode>(parser.GetTokenizer(), NK_Expression);
        expr->ty = std::make_unique<Type>(Types[FLOAT]);
        expr->op = OP_CONST;
        expr->val = parser.GetTokenizer().GetTokenVal();
        parser.NextToken();

        return expr;
    }



    case TK_ID:
    {
        auto expr = std::make_shared<ExpressionNode>(parser.GetTokenizer(), NK_Expression);
        expr->op = OP_ID;
        expr->val = parser.GetTokenizer().GetTokenVal();
        parser.NextToken();

        return expr;
    }

	case TK_FLOATCONST:
    {
        auto expr = std::make_shared<ExpressionNode>(parser.GetTokenizer(), NK_Expression);
        expr->ty  = std::make_unique<Type>(Types[FLOAT]);
        expr->op  = OP_CONST;
        expr->val = parser.GetTokenizer().GetTokenVal();
        parser.NextToken();

        return expr;
    }

	case TK_STRING:
    {
        auto expr = std::make_shared<ExpressionNode>(parser.GetTokenizer(), NK_Expression);
        expr->ty = ArrayOf(strlen((char*)(parser.GetTokenizer().GetTokenVal().p)) + 1, Types[CHAR]);
        expr->op = OP_STR;
        expr->val = parser.GetTokenizer().GetTokenVal();
        parser.NextToken();

        return expr;
    }

	case TK_LPAREN:
    {
        parser.NextToken();
        auto expr = ParseExpression(parser);
        parser.Expect(TK_RPAREN);
        parser.NextToken();

        return expr;
    }

    case TK_LBRACE:
    {
        parser.NextToken();
        auto expr = ParseExpression(parser);
        parser.Expect(TK_RBRACE);
        parser.NextToken();

        return expr;
    }

	default:
    {
        assert(0);
//        Error(&TokenCoord, "Expect identifier, string, constant or (");
        return nullptr;
    }
	}
}

/**
 *  postfix-expression:
 *		primary-expression
 *		postfix-expression [ expression ]
 *		postfix-expression ( [argument-expression-list] )
 *		postfix-expression --> expression
 *		postfix-expression | selector expression
 *		postfix-expression : expression
 *		postfix-expression *
*/
ExprNodePtr ExpressionParser::ParsePostfixExpression(Parser& parser)
{
	auto expr = ParsePrimaryExpression(parser);
	while (1)
	{
		switch (parser.CurrTokenType())
		{
		case TK_LBRACKET:
        {
            auto p = std::make_shared<ExpressionNode>(parser.GetTokenizer(), NK_Expression);
            p->op = OP_INDEX;
            p->kids[0] = expr;
            parser.NextToken();
            p->kids[1] = ParseExpression(parser);
            parser.Expect(TK_RBRACKET);
            parser.NextToken();

            expr = p;
        }
			break;

		case TK_LPAREN:
        {
            auto p = std::make_shared<ExpressionNode>(parser.GetTokenizer(), NK_Expression);
            p->op = OP_CALL;
            p->kids[0] = expr;
            parser.NextToken();
            if (parser.CurrTokenType() != TK_RPAREN)
            {
                NodePtr* tail = nullptr;

                p->kids[1] = ParseAssignmentExpression(parser);
                tail = &p->kids[1]->next;
                while (parser.CurrTokenType() == TK_COMMA)
                {
                    parser.NextToken();
                    *tail = ParseAssignmentExpression(parser);
                    tail = &(*tail)->next;
                }
            }
            parser.Expect(TK_RPAREN);
            parser.NextToken();

            expr = p;
        }
			break;

        case TK_MUL:
        {
            auto p = std::make_shared<ExpressionNode>(parser.GetTokenizer(), NK_Expression);
            p->op = OP_REPEAT;
            p->kids[0] = expr;
            parser.NextToken();

            expr = p;
        }
            break;

        case TK_DOT:
        {
            auto p = std::make_shared<ExpressionNode>(parser.GetTokenizer(), NK_Expression);
            p->op = OP_MEMBER;
            p->kids[0] = expr;
            parser.NextToken();
            if (parser.CurrTokenType() != TK_ID)
            {
                assert(0);
//				Error(&p->coord, "Expect identifier as struct or union member");
            }
            else
            {
                p->val = parser.GetTokenizer().GetTokenVal();
                parser.NextToken();
            }

            expr = p;
        }
            break;

		default:
			return expr;
		}
	}
}

/**
 *  unary-expression:
 *		postfix-expression
 *		unary-operator unary-expression
 *		( type-name ) unary-expression
 *
 *  unary-operator:
 *		++ -- & * + - ! ~
 */
ExprNodePtr ExpressionParser::ParseUnaryExpression(Parser& parser)
{
	switch (parser.CurrTokenType())
	{
	case TK_SUB:
	case TK_NOT:
	case TK_COMP:
    case TK_RELATIVE:
    {
        auto expr = std::make_shared<ExpressionNode>(parser.GetTokenizer(), NK_Expression);
        expr->op = UNARY_OP(parser.CurrTokenType());
        assert(expr->op != OP_NONE);
        parser.NextToken();
        expr->kids[0] = ParseUnaryExpression(parser);

        return expr;
    }
        break;

	case TK_LPAREN:
    {
        auto t = parser.PeekToken();
        //if (DeclarationParser::IsTypeName(t.GetType()))
        //{
        //    auto expr = std::make_shared<ExpressionNode>(parser.GetTokenizer(), NK_Expression);
        //    expr->op = OP_CAST;
        //    parser.NextToken();
        //    auto node = DeclarationParser::ParseTypeName(parser);
        //    expr->kids[0] = std::make_shared<ExpressionNode>(*node);
        //    parser.Expect(TK_RPAREN);
        //    parser.NextToken();
        //    expr->kids[1] = ParseUnaryExpression(parser);

        //    return expr;
        //}
        //else
        {
            return ParsePostfixExpression(parser);
        }
    }
		break;

	default:
		return ParsePostfixExpression(parser);
	}
}

/**
 * Parse a binary expression, from logical-OR-expresssion to multiplicative-expression
 */
ExprNodePtr ExpressionParser::ParseBinaryExpression(Parser& parser, int prec)
{
    int new_prec = 0;
    auto expr = ParseUnaryExpression(parser);
    while (IsBinaryOP(parser.CurrTokenType()) && (new_prec = Prec[BINARY_OP(parser.CurrTokenType())]) >= prec)
    {
        auto bin_expr = std::make_shared<ExpressionNode>(parser.GetTokenizer(), NK_Expression);
        bin_expr->op = BINARY_OP(parser.CurrTokenType());
        bin_expr->kids[0] = expr;
        parser.NextToken();
        bin_expr->kids[1] = ParseBinaryExpression(parser, new_prec + 1);

        expr = bin_expr;
    }

    return expr;

}

}