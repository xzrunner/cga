#ifndef TOKENOP
#error "You must define TOKENOP macro before include this file"
#endif

TOKENOP(TK_ASSIGN,        OP_ASSIGN,        OP_NONE)
TOKENOP(TK_OR,            OP_OR,            OP_NONE)
TOKENOP(TK_AND,           OP_AND,           OP_NONE)
TOKENOP(TK_SEPARATOR,     OP_SEPARATOR,     OP_NONE)
TOKENOP(TK_COLON,         OP_COLON,         OP_NONE)
TOKENOP(TK_EQUAL,         OP_EQUAL,         OP_NONE)
TOKENOP(TK_UNEQUAL,       OP_UNEQUAL,       OP_NONE)
TOKENOP(TK_GREAT,         OP_GREAT,         OP_NONE)
TOKENOP(TK_LESS,          OP_LESS,          OP_NONE)
TOKENOP(TK_GREAT_EQ,      OP_LESS_EQ,       OP_NONE)
TOKENOP(TK_LESS_EQ,       OP_GREAT_EQ,      OP_NONE)
TOKENOP(TK_ADD,           OP_ADD,           OP_NONE)
TOKENOP(TK_SUB,           OP_SUB,           OP_NEG)
TOKENOP(TK_MUL,           OP_MUL,           OP_NONE)
TOKENOP(TK_DIV,           OP_DIV,           OP_NONE)
TOKENOP(TK_MOD,           OP_MOD,           OP_NONE)
TOKENOP(TK_RULE,          OP_RULE,          OP_NONE)
TOKENOP(TK_NOT,           OP_NONE,          OP_NOT)
TOKENOP(TK_COMP,          OP_NONE,          OP_COMP)
TOKENOP(TK_RELATIVE,      OP_NONE,          OP_RELATIVE)
