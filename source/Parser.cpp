#include "cga/Parser.h"
#include "cga/AST.h"

namespace cga
{

Parser::Parser(const char* str, const std::shared_ptr<StringPool>& str_pool)
    : m_tokenizer(str, str_pool)
{
    m_curr_token = m_tokenizer.NextToken();
}

//void Parser::Parse()
//{
////    TokenType token;
//
//}

void Parser::NextToken()
{
    m_curr_token = m_tokenizer.NextToken();
}

lexer::TokenTemplate<TokenType>
Parser::PeekToken()
{
    return m_tokenizer.PeekToken();
}

void Parser::SkipTo(int toks[])
{
	int *p = toks;
//	Coord cord(m_tokenizer);

    if (ASTHelper::CurrentTokenIn(CurrTokenType(), toks) ||
        m_curr_token.GetType() == TK_END) {
        return;
    }

//	cord = TokenCoord;
	while (CurrTokenType() != TK_END)
	{
		p = toks;
		while (*p)
		{
            if (CurrTokenType() == *p) {
                return;
            }
			p++;
		}
		NextToken();
	}
}

void Parser::Expect(const TokenType mask)
{
    if (m_curr_token.GetType() != mask) {
        throw lexer::ParserException(m_curr_token.Line(), m_curr_token.Column(),
            ExpectString(TokenName(mask), m_curr_token));
    }
}

bool Parser::IsTypedefName(const std::string& id) const
{
    return m_typedef_names.find(id) != m_typedef_names.end();
}

}