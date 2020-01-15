#pragma once

#define FIRST_EXPRESSION                                                          \
    TK_ID,         TK_FLOATCONST, TK_STRING,     TK_ADD,      TK_CONST, TK_ATTR,  \
    TK_SUB,        TK_MUL,        TK_NOT,        TK_COMP,     TK_RELATIVE,  TK_LPAREN

#define FIRST_STATEMENT                                                           \
    TK_RULE, TK_CASE, TK_ELSE, TK_LBRACE, FIRST_EXPRESSION
