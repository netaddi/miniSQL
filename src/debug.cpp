#include "stdafx.h"

void printData(char * data, int length)
{
    for (int i = 0; i < length; i++)
    {
        cout << static_cast<int>(data[i]) << " ";
    }
    cout << endl;
}
