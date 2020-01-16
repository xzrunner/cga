#include "cgac/Statement.h"
#include "cgac/Parser.h"
#include "cgac/grammer.h"
#include "cgac/Expression.h"
#include "cgac/Expression.h"

namespace cgac
{

StmtNodePtr StatementParser::ParseStatement(Parser& parser)
{
    auto top_comp = std::make_shared<CompoundStmtNode>(parser.GetTokenizer(), NK_CompoundStatement);
    auto* top_tail = &top_comp->stmts;

    std::shared_ptr<RuleStmtNode> rule_stmt = nullptr;
    NodePtr* rule_tail = nullptr;

    std::shared_ptr<CaseStmtNode> case_stmt = nullptr;
    NodePtr* case_tail = nullptr;

    std::shared_ptr<ElseStmtNode> else_stmt = nullptr;
    NodePtr* else_tail = nullptr;

    auto append_expr = [&](const StmtNodePtr& expr_stat)
    {
        if (case_stmt)
        {
            assert(case_tail && rule_stmt && !else_stmt);
            *case_tail = expr_stat;
            case_tail = &(*case_tail)->next;
        }
        else if (else_stmt)
        {
            assert(else_tail && rule_stmt && !case_stmt);
            *else_tail = expr_stat;
            else_tail = &(*else_tail)->next;
        }
        else if (rule_stmt)
        {
            assert(rule_tail);
            *rule_tail = expr_stat;
            rule_tail = &(*rule_tail)->next;
        }
        else
        {
            *top_tail = expr_stat;
            top_tail = &(*top_tail)->next;
        }
    };

    while (parser.CurrTokenType() != TK_END)
    {
        switch (parser.CurrTokenType())
        {
        case TK_CASE:
        {
            assert(!else_stmt);
            // flush case
            if (case_stmt)
            {
                assert(rule_tail);
                *rule_tail = case_stmt;
                rule_tail = &(*rule_tail)->next;

                case_stmt.reset();
                case_tail = nullptr;
            }

            case_stmt = std::make_shared<CaseStmtNode>(parser.GetTokenizer(), NK_CaseStatement);
            case_tail = &case_stmt->stmts;

            parser.NextToken();

            case_stmt->expr = ExpressionParser::ParsePrimaryExpression(parser);
            parser.Expect(TK_COLON);
            parser.NextToken();

            continue;
        }
            break;

        case TK_ELSE:
        {
            // flush case
            assert(case_stmt && case_tail && rule_tail);
            *rule_tail = case_stmt;
            rule_tail = &(*rule_tail)->next;
            case_stmt.reset();
            case_tail = nullptr;

            else_stmt = std::make_shared<ElseStmtNode>(parser.GetTokenizer(), NK_ElseStatement);
            else_tail = &else_stmt->stmts;

            parser.NextToken();
            parser.Expect(TK_COLON);
            parser.NextToken();

            continue;
        }
            break;

        case TK_LBRACE:
        {
            parser.NextToken();

            auto sel_stmt = std::make_shared<SelectorStmtNode>(parser.GetTokenizer(), NK_SelectorStatement);
            sel_stmt->expr = ExpressionParser::ParseExpression(parser);

            append_expr(sel_stmt);

            parser.Expect(TK_RBRACE);
            parser.NextToken();

            if (parser.CurrTokenType() == TK_MUL) {
                sel_stmt->repeat = true;
                parser.NextToken();
            }

            continue;
        }
            break;
        }

        auto expr_stat = ParseExpressionStatement(parser);
        auto expr = std::static_pointer_cast<ExprStmtNode>(expr_stat)->expr;
        if (expr->op == OP_CONST) {
            continue;
        }
        if (parser.CurrTokenType() == TK_RULE)
        {
            // flush else
            if (else_stmt)
            {
                assert(rule_stmt && rule_tail);

                *rule_tail = else_stmt;
                rule_tail = &(*rule_tail)->next;

                else_stmt.reset();
                else_tail = nullptr;
            }

            // flush rule
            if (rule_stmt)
            {
                *top_tail = rule_stmt;
                top_tail = &(*top_tail)->next;

                rule_stmt.reset();
            }
            assert(!rule_stmt);

            rule_stmt = std::make_shared<RuleStmtNode>(parser.GetTokenizer(), NK_RuleStatement);
            rule_tail = &rule_stmt->stmts;

            switch (expr->op)
            {
            case OP_ID:
                rule_stmt->name = static_cast<const char*>(expr->val.p);
                break;
            case OP_CALL:
                assert(expr->kids[0] && expr->kids[0]->op == OP_ID);
                rule_stmt->name = static_cast<const char*>(expr->kids[0]->val.p);
                rule_stmt->params = expr->kids[1];
                break;
            default:
                assert(0);
            }

            parser.NextToken();
        }
        else
        {
            // flush rule
            if (expr->op == OP_ASSIGN && rule_stmt)
            {
                *top_tail = rule_stmt;
                top_tail = &(*top_tail)->next;

                rule_stmt.reset();
            }

            append_expr(expr_stat);
        }
    }

    assert(!case_stmt);

    // flush else
    if (else_stmt)
    {
        assert(rule_stmt && rule_tail);

        *rule_tail = else_stmt;
        rule_tail = &(*rule_tail)->next;

        else_stmt.reset();
        else_tail = nullptr;
    }

    // flush rule
    if (rule_stmt)
    {
        *top_tail = rule_stmt;
        top_tail = &(*top_tail)->next;

        rule_stmt.reset();
    }

    return top_comp;
}

StmtNodePtr StatementParser::ParseExpressionStatement(Parser& parser)
{
    auto expr_stmt = std::make_shared<ExprStmtNode>(parser.GetTokenizer(), NK_ExpressionStatement);
    expr_stmt->expr = ExpressionParser::ParseExpression(parser);
	return expr_stmt;
}

}