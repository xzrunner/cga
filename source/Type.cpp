#include "cgac/Type.h"

#include <assert.h>

#define CHAR_SIZE         1
#define FLOAT_SIZE        4

namespace cgac
{

Type Types[TYPE_COUNT];

std::unique_ptr<Type> ArrayOf(int len, const Type& ty)
{
    auto aty = std::make_unique<Type>();

    aty->categ = ARRAY;
    aty->qual = 0;
    aty->size = len * ty.size;
    aty->align = ty.align;
    aty->b_categ = ty.categ;

    return aty;
}

std::string TypeToString(const Type& ty)
{
    char *names[] =
    {
        "char", "float"
    };

    if (ty.categ >= CHAR && ty.categ < TYPE_COUNT) {
        return names[ty.categ];
    }

    switch (ty.categ)
    {
    case ARRAY:
        //return FormatName("%s[%d]", TypeToString(ty.bty), ty.size / ty.bty->size);
        return "array";

    default:
        assert(0);
        return nullptr;
    }
}

void SetupTypeSystem(void)
{
    Types[CHAR].size  = CHAR_SIZE;
    Types[FLOAT].size = FLOAT_SIZE;

    for (int i = CHAR; i < TYPE_COUNT; ++i)
    {
        Types[i].categ = i;
        Types[i].align = Types[i].size;
    }
}

}