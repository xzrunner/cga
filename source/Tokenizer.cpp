#include "cgac/Tokenizer.h"
#include "cgac/keyword.h"

namespace cgac
{

char* Tokenizer::TokenStrings[] =
{
    "begin",
#define TOKEN(k, s) s,
#include "cgac/token_cfg.h"
#undef  TOKEN
};

Tokenizer::Tokenizer(const char* str)
    : lexer::Tokenizer<TokenType>(str, str + strlen(str), "\"", '\\')
{
    InitScanners();
}

lexer::Tokenizer<TokenType>::Token
Tokenizer::PeekToken()
{
    auto old_val = m_token_val;
    auto ret = lexer::Tokenizer<TokenType>::PeekToken();
    m_token_val = old_val;
    return ret;
}

lexer::Tokenizer<TokenType>::Token
Tokenizer::EmitToken()
{
    DiscardWhite();

    size_t start_line = Line();
    size_t start_column = Column();
    if (Eof())
    {
        return Token(TK_END, nullptr, nullptr, Length(), start_line, start_column);
    }
    else
    {
        const char* begin_pos = CurPos();
        auto token = m_scanners[(const unsigned char)(*begin_pos)]();
        const char* end_pos = CurPos();
        return Token(token, begin_pos, end_pos, Offset(begin_pos), start_line, start_column);
    }
}

void Tokenizer::DiscardWhite()
{
    DiscardWhile(Whitespace());
    DiscardComments('#');
    DiscardWhile(Whitespace());
}

void Tokenizer::InitScanners()
{
    for (int i = 0, n = END_OF_FILE + 1; i < n; ++i)
    {
        if (IsLetter(i)) {
            m_scanners[i] = std::bind(&Tokenizer::ScanIdentifier, this);
        } else if (IsDigit(i)) {
            m_scanners[i] = std::bind(&Tokenizer::ScanNumericLiteral, this);
        } else {
            m_scanners[i] = std::bind(&Tokenizer::ScanBadChar, this);
        }
    }

    m_scanners[END_OF_FILE] = std::bind(&Tokenizer::ScanEOF, this);
    m_scanners['\''] = std::bind(&Tokenizer::ScanRelative, this);
    m_scanners['"'] = std::bind(&Tokenizer::ScanStringLiteral, this);
    m_scanners['+'] = std::bind(&Tokenizer::ScanADD, this);
    m_scanners['-'] = std::bind(&Tokenizer::ScanMinus, this);
    m_scanners['*'] = std::bind(&Tokenizer::ScanMUL, this);
    m_scanners['/'] = std::bind(&Tokenizer::ScanDIV, this);
    m_scanners['<'] = std::bind(&Tokenizer::ScanLess, this);
    m_scanners['>'] = std::bind(&Tokenizer::ScanGreat, this);
    m_scanners['='] = std::bind(&Tokenizer::ScanASSIGN, this);
    m_scanners['|'] = std::bind(&Tokenizer::ScanSEPARATOR, this);
    m_scanners['{'] = std::bind(&Tokenizer::ScanLBRACE, this);
    m_scanners['}'] = std::bind(&Tokenizer::ScanRBRACE, this);
    m_scanners['('] = std::bind(&Tokenizer::ScanLPAREN, this);
    m_scanners[')'] = std::bind(&Tokenizer::ScanRPAREN, this);
    m_scanners[','] = std::bind(&Tokenizer::ScanCOMMA, this);
    m_scanners['~'] = std::bind(&Tokenizer::ScanCOMP, this);
    m_scanners[':'] = std::bind(&Tokenizer::ScanCOLON, this);
    m_scanners['|'] = std::bind(&Tokenizer::ScanBar, this);
    m_scanners['&'] = std::bind(&Tokenizer::ScanAmpersand, this);
    m_scanners['.'] = std::bind(&Tokenizer::ScanDot, this);
}

TokenType Tokenizer::ScanIdentifier()
{
    auto start = CurPos();
    Advance();
    while (IsLetterOrDigit(CurChar())) {
        Advance();
    }

    int tok = FindKeyword(start, (int)(CurPos() - start));
    if (tok == TK_ID) {
        m_token_val.p = (void*)(m_str_pool.InsertAndQuery(start, (int)(CurPos() - start)));
    }

    return static_cast<TokenType>(tok);
}

TokenType Tokenizer::ScanNumericLiteral()
{
    auto num_delim = Whitespace() + ")]},";

    auto start = CurPos();
    auto end = ReadDecimal(num_delim);
    if (end != nullptr)
    {
        m_token_val.f = strtof(start, nullptr);
        return TK_FLOATCONST;
    }

    assert(0);
    return TK_BEGIN;
}

TokenType Tokenizer::ScanBadChar()
{
    assert(0);
//    Error(&TokenCoord, "illegal character:\\x%x", CurChar());
    return NextToken().GetType();
}

TokenType Tokenizer::ScanEOF()
{
    return TK_END;
}

TokenType Tokenizer::ScanRelative()
{
    Advance();
    return TK_RELATIVE;
}

TokenType Tokenizer::ScanStringLiteral()
{
    Advance();
    const char* start_pos = CurPos();
    const char* end_pos = ReadQuotedString();
    m_token_val.p = (void*)(m_str_pool.InsertAndQuery(start_pos, end_pos - start_pos));

	return TK_STRING;
}

TokenType Tokenizer::ScanMinus()
{
    Advance();
    if (CurChar() == '-')
    {
        Advance();
        if (CurChar() == '>')
        {
            Advance();
            return TK_RULE;
        }
    }
    return TK_SUB;
}

TokenType Tokenizer::ScanBar()
{
    Advance();
    if (CurChar() == '|')
    {
        Advance();
        return TK_OR;
    }
    else
    {
        return TK_SEPARATOR;
    }
}

TokenType Tokenizer::ScanAmpersand()
{
    Advance();
    if (CurChar() == '&')
    {
        Advance();
        return TK_AND;
    }
    else
    {
        assert(0);
        return TK_END;
    }
}

TokenType Tokenizer::ScanDot()
{
    if (IsDigit(NextChar()))
    {
        assert(0);
        return TK_END;
    }
    else
    {
        Advance();
        return TK_DOT;
    }
}

TokenType Tokenizer::ScanLess()
{
    Advance();
    if (CurChar() == '=')
    {
        Advance();
        return TK_LESS_EQ;
    }
    else
    {
        return TK_LESS;
    }
}

TokenType Tokenizer::ScanGreat()
{
    Advance();
    if (CurChar() == '=')
    {
        Advance();
        return TK_GREAT_EQ;
    }
    else
    {
        return TK_GREAT;
    }
}

#define SINGLE_CHAR_SCANNER(t) \
TokenType Tokenizer::Scan##t() \
{                              \
    Advance();                 \
    return TK_##t;             \
}

SINGLE_CHAR_SCANNER(ADD)
SINGLE_CHAR_SCANNER(MUL)
SINGLE_CHAR_SCANNER(DIV)
SINGLE_CHAR_SCANNER(ASSIGN)
SINGLE_CHAR_SCANNER(SEPARATOR)
SINGLE_CHAR_SCANNER(LBRACE)
SINGLE_CHAR_SCANNER(RBRACE)
SINGLE_CHAR_SCANNER(LPAREN)
SINGLE_CHAR_SCANNER(RPAREN)
SINGLE_CHAR_SCANNER(COMMA)
SINGLE_CHAR_SCANNER(COMP)
SINGLE_CHAR_SCANNER(COLON)

int Tokenizer::FindKeyword(const char* str, int len) const
{
    struct keyword *p = NULL;
    int index = 0;

    if (IsLetter(*str) && *str != '_') {
        index = ToUpper(*str) - 'A' + 1;
    }

    p = keywords[index];
    while (p->name)
    {
        if (p->len == len && strncmp((const char*)(str), p->name, len) == 0) {
            break;
        }
        p++;
    }
    return p->tok;
}

}