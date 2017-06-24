#include "element.h"

ostream & operator << (ostream& os, const Element & e)
{
    switch (e.type)
    {
        case INT:    os << e.intData;     return os;
        case FLOAT:  os << e.floatData;   return os;
        case STRING: os << e.stringData;  return os;
    }
    return os;
}

bool operator < (Element & lhs, Element & rhs)
{
    // if (lhs.type == rhs.type)
    // {
    //     switch (lhs.type)
    //     {
    //         case INT:     return lhs.intData    < rhs.intData;
    //         case FLOAT:   return lhs.floatData  < rhs.floatData;
    //         case STRING:  return lhs.stringData < rhs.stringData;
    //     }
    // }
    //
    // if (lhs.type == STRING || rhs.type == STRING)
    // {
    //     return false;
    // }

    // if (lhs.type == FLOAT && rhs.type == int)
    // {
    //     /* code */
    // }

    switch (lhs.type * 3 + rhs.type)
    {
        case 0 : return lhs.intData    < rhs.intData;
        case 1 : return lhs.intData    < rhs.floatData;
        case 2 : return false;
        case 3 : return lhs.floatData  < rhs.intData;
        case 4 : return lhs.floatData  < rhs.intData;
        case 5 : return false;
        case 6 : return false;
        case 7 : return false;
        case 8 : return lhs.stringData < rhs.stringData;
    }
    return false;
}

bool operator > (Element & lhs, Element & rhs)
{
    return rhs < lhs;
}

bool operator <= (Element & lhs, Element & rhs)
{
    return !(lhs > rhs);
}

bool operator >= (Element & lhs, Element & rhs)
{
    return !(lhs < rhs);
}

bool operator == (Element & lhs, Element & rhs)
{
    return (!(lhs > rhs)) && (!(lhs < rhs));
}

bool operator != (Element & lhs, Element & rhs)
{
    return !(lhs == rhs);
}
