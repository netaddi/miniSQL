#include "../src/stdafx.h"
#include "../src/debug.h"
#include "../src/element.h"

int main(int argc, char const *argv[])
{
    Element f((float)123.456);
    Element i(123456);
    Element s("abcdefg");
    cout << f << i << s << endl;

    char * fb, * ib, * sb;

    fb = f.getBitToBuffer();
    ib = i.getBitToBuffer();
    sb = s.getBitToBuffer();

    Element f1(FLOAT,  sizeof(float), fb);
    Element i1(INT,    sizeof(int),   ib);
    Element s1(STRING, 8,            sb);
    cout << f1 << i1 << s1 << endl;

    return 0;


}
