#pragma once

namespace cgac
{

typedef unsigned int TokenType;

enum Token
{
	TK_BEGIN = 0,
#define TOKEN(k, s) k,
#include "cgac/token_cfg.h"
#undef  TOKEN

}; // Token

}