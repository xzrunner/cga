#pragma once

namespace cga
{

typedef unsigned int TokenType;

enum Token
{
	TK_BEGIN = 0,
#define TOKEN(k, s) k,
#include "cga/token_cfg.h"
#undef  TOKEN

}; // Token

}