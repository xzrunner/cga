#ifndef TOKEN
#error "You must define TOKEN macro before include this file"
#endif

// keywords
TOKEN(TK_ATTR,          "attr")
TOKEN(TK_CONST,         "const")
TOKEN(TK_CASE,          "case")
TOKEN(TK_ELSE,          "else")

//identifier
TOKEN(TK_ID,            "ID")

// constant
TOKEN(TK_FLOATCONST,    "float")
TOKEN(TK_STRING,        "STR")

// operators
TOKEN(TK_COMMA,         ",")
TOKEN(TK_QUESTION,      "?")
TOKEN(TK_ASSIGN,        "=")
TOKEN(TK_OR,            "||")
TOKEN(TK_AND,           "&&")
TOKEN(TK_SEPARATOR,     "|")
TOKEN(TK_COLON,         ":")
TOKEN(TK_EQUAL,         "==")
TOKEN(TK_UNEQUAL,       "!=")
TOKEN(TK_GREAT,         ">")
TOKEN(TK_LESS,          "<")
TOKEN(TK_GREAT_EQ,      ">=")
TOKEN(TK_LESS_EQ,       "<=")
TOKEN(TK_ADD,           "+")
TOKEN(TK_SUB,           "-")
TOKEN(TK_MUL,           "*")
TOKEN(TK_DIV,           "/")
TOKEN(TK_MOD,           "%")
TOKEN(TK_RULE,          "-->")
TOKEN(TK_NOT,           "!")
TOKEN(TK_COMP,          "~")
TOKEN(TK_LPAREN,        "(")
TOKEN(TK_RPAREN,        ")")
TOKEN(TK_LBRACKET,      "[")
TOKEN(TK_RBRACKET,      "]")

//punctuators
TOKEN(TK_LBRACE,        "{")
TOKEN(TK_RBRACE,        "}")
TOKEN(TK_NEWLINE,       "\n")

TOKEN(TK_COMMENTS,      "#")
TOKEN(TK_END,           "EOF")