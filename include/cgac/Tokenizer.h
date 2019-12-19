#pragma once

#include "cgac/Token.h"
#include "cgac/StringPool.h"

#include <lexer/Tokenizer.h>

#include <functional>
#include <array>

namespace cgac
{

union Value
{
	float f;
	void* p;
};

class Tokenizer : public lexer::Tokenizer<unsigned int>
{
public:
    Tokenizer(const char* str);

    virtual Token PeekToken() override;

    auto& GetTokenVal() const { return m_token_val; }

protected:
    virtual Token EmitToken() override;

private:
    void InitScanners();

    TokenType ScanIdentifier();
    TokenType ScanNumericLiteral();
    TokenType ScanBadChar();
    TokenType ScanEOF();
    TokenType ScanStringLiteral();
    TokenType ScanADD();
    TokenType ScanMinus();
    TokenType ScanMUL();
    TokenType ScanDIV();
    TokenType ScanASSIGN();
    TokenType ScanSEPARATOR();
    TokenType ScanLBRACE();
    TokenType ScanRBRACE();
    TokenType ScanLPAREN();
    TokenType ScanRPAREN();
    TokenType ScanCOLON();
    TokenType ScanBar();
    TokenType ScanAmpersand();

    int FindKeyword(const char* str, int len) const;

private:
    static const unsigned char END_OF_FILE = 255;

public:
    static char* TokenStrings[];

private:
    bool m_skip_eol;

    std::array<std::function<TokenType(void)>, END_OF_FILE + 1> m_scanners;

    Value m_token_val;

    StringPool m_str_pool;

}; // Tokenizer

}