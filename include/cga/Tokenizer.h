#pragma once

#include "cga/Token.h"
#include "cga/StringPool.h"

#include <lexer/Tokenizer.h>

#include <functional>
#include <array>

namespace cga
{

union Value
{
	float f;
	void* p;
};

class Tokenizer : public lexer::Tokenizer<unsigned int>
{
public:
    Tokenizer(const char* str,
        const std::shared_ptr<StringPool>& str_pool = nullptr);

    virtual Token PeekToken() override;

    auto& GetTokenVal() const { return m_token_val; }

protected:
    virtual Token EmitToken() override;

private:
    void DiscardWhite();

    void InitScanners();

    TokenType ScanIdentifier();
    TokenType ScanNumericLiteral();
    TokenType ScanBadChar();
    TokenType ScanEOF();
    TokenType ScanRelative();
    TokenType ScanStringLiteral();
    TokenType ScanADD();
    TokenType ScanMinus();
    TokenType ScanMUL();
    TokenType ScanDIV();
    TokenType ScanMOD();
    TokenType ScanLess();
    TokenType ScanGreat();
    TokenType ScanEqual();
    TokenType ScanSEPARATOR();
    TokenType ScanLBRACE();
    TokenType ScanRBRACE();
    TokenType ScanLPAREN();
    TokenType ScanRPAREN();
    TokenType ScanCOMMA();
    TokenType ScanCOMP();
    TokenType ScanCOLON();
    TokenType ScanBar();
    TokenType ScanAmpersand();
    TokenType ScanDot();

    int FindKeyword(const char* str, int len) const;

private:
    static const unsigned char END_OF_FILE = 255;

public:
    static char* TokenStrings[];

private:
    bool m_skip_eol;

    std::array<std::function<TokenType(void)>, END_OF_FILE + 1> m_scanners;

    Value m_token_val;

    std::shared_ptr<StringPool> m_str_pool = nullptr;

}; // Tokenizer

}