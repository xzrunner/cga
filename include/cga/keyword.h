#pragma once

namespace cga
{

struct keyword
{
	char *name;
	int len;
	int tok;
};

static struct keyword keywords_[] =
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsA[] =
{
    {"attr", 4, TK_ATTR},
	{NULL,   0, TK_ID}
};

static struct keyword keywordsB[] =
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsC[] =
{
	{"case",  4, TK_CASE},
	{"const", 5, TK_CONST},
	{NULL,    0, TK_ID}
};

static struct keyword keywordsD[] =
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsE[] =
{
	{"else", 4, TK_ELSE},
	{NULL,   0, TK_ID}
};

static struct keyword keywordsF[] =
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsG[] =
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsH[] =
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsI[] =
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsJ[] =
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsK[] =
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsL[] =
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsM[] =
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsN[] =
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsO[] =
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsP[] =
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsQ[] =
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsR[] =
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsS[] =
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsT[] =
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsU[] =
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsV[] =
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsW[] =
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsX[] =
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsY[] =
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsZ[] =
{
	{NULL, 0, TK_ID}
};

static struct keyword *keywords[] =
{
	keywords_, keywordsA, keywordsB, keywordsC,
	keywordsD, keywordsE, keywordsF, keywordsG,
	keywordsH, keywordsI, keywordsJ, keywordsK,
	keywordsL, keywordsM, keywordsN, keywordsO,
	keywordsP, keywordsQ, keywordsR, keywordsS,
	keywordsT, keywordsU, keywordsV, keywordsW,
	keywordsX, keywordsY, keywordsZ
};

}