#pragma once

#include <string>

namespace cgac
{

enum
{
	CHAR, FLOAT,

    TYPE_COUNT,

    ARRAY
};

struct Type
{
    int categ : 8;
    int qual  : 8;
    int align : 16;
    int size  : 24;

    int b_categ : 8;
};

std::unique_ptr<Type> ArrayOf(int len, const Type& ty);

std::string TypeToString(const Type& ty);

void SetupTypeSystem(void);

extern Type Types[TYPE_COUNT];

}