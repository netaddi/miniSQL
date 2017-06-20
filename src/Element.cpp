#include "Element.h"

bool operator<(const Element &v1, const Element &v2)
{
    if (v1.type != v2.type)
    {
        return false;
    }
    switch (v1.type)
    {
        case INT: return v1.intData < v2.intData;
        case FLOAT : return v1.floatData < v2.floatData;
        default : return v1.stringData < v2.stringData;
    }
}

bool operator>(const Element &v1, const Element &v2)
{
    return v2 < v1;
}

bool operator>=(const Element &v1, const Element &v2)
{
    return !(v1 < v2);
}

bool operator<=(const Element &v1, const Element &v2)
{
    return !(v1 > v2);
}

bool operator==(const Element &v1, const Element &v2)
{
    return ((v1 >= v2) && (!(v1 > v2)));
}

bool operator!=(const Element &v1, const Element &v2)
{
    return (v1 < v2) || (v1 > v2);
}
