#pragma once

#include "cgac/Tokenizer.h"

#include <lexer/Parser.h>

namespace cgac
{

class Parser : public lexer::Parser<TokenType>
{
public:
    Parser(const char* str,
        const std::shared_ptr<StringPool>& str_pool = nullptr);

    auto& GetTokenizer() const { return m_tokenizer; }

    void NextToken();
    auto CurrTokenType() const { return m_curr_token.GetType(); }

    lexer::TokenTemplate<TokenType> PeekToken();

    void SkipTo(int toks[]);

    void Expect(const TokenType mask);

    bool IsTypedefName(const std::string& id) const;

private:
    virtual std::map<TokenType, std::string> TokenNames() const {
        return std::map<TokenType, std::string>();
    }

private:
    typedef Tokenizer::Token Token;

    cgac::Tokenizer m_tokenizer;

    lexer::TokenTemplate<TokenType> m_curr_token;

    std::set<std::string> m_typedef_names;

}; // Parser

}