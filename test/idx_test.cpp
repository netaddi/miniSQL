#include "../src/IndexManager.h"
#include "../src/element.h"

int main(int argc, char const *argv[])
{
    IndexManager idxMan;

    IndexInfo idx0("testTable", "atr0", "idx0");

    idxMan.createIndex(idx0);

    for (int i = 0; i < 10; i++)
    {
        idxMan.insertIntoIndex(idx0, Element(i), i * 20);
    }
    for (int i = 0; i < 10; i++)
    {
        cout << "query "<< to_string(i) << ": " << idxMan.queryFromIndex("testTable", "atr0", new Element(i)) << '\n';
    }

    // cout << "query 21: " << idxMan.queryFromIndex("testTable", "atr0", new Element((int)21));


    return 0;
}
