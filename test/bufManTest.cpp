#include "../src/BufferManager.h"
#include "../src/stdafx.h"
#include "../src/element.h"
// #include "../src/debug.h"

int main(int argc, char const *argv[])
{

    // BufferPage testBP("testTable", 24, 0);
    // BufferPage testBP1("testTable", 24, 1);
    //
    //
    // testBP.usedLength = 12345678;
    // cout << testBP.writeBackPage() << endl;
    //
    // BufferPool bpl("testTable1", 0x12678);
    // bpl.addNewBufferToPool();
    // bpl.addNewBufferToPool();
    // cout << "pool" << bpl.writeBackBuffers() << endl;


    BufferManager bufMan;
    TableInfo t("testTable", "id",
                {Attribute("id", "int", true),
                 Attribute("user", 8, true),
                 Attribute("password", 8, false)});
    bufMan.createBufferForTable(t);

    char newData[] = "\0\0\0\0usernamepassword";
    printData(newData, 20);

    for (int i = 0; i < 10; i++)
    {
        newData[0] = (char)i;
        bufMan.insertIntoTable("testTable", newData);
        // bufMan.writeBackTable("testTable");
    }
    cout << "deletion offset: " << bufMan.deleteFromTableWithOffset("testTable", 30) << endl;

    cout << bufMan.bufferMap["testTable"] -> buffers.size() << endl;
    cout << bufMan.bufferMap["testTable"] -> activePageId << endl;
    cout << bufMan.bufferMap["testTable"] -> dirty << endl;
    // for (int i = 0; i < 10000; i++)
    // {
    //     cout << bufMan.queryTableWithOffset("testTable", i) << endl;
    // }
    cout << bufMan.queryTableWithOffset("testTable", 4) << endl;
    cout << bufMan.queryTableWithOffset("testTable", 12) << endl;
    cout << bufMan.queryTableWithOffset("testTable", 20) << endl;

    printData(bufMan.queryCompleteTable("testTable"), 60);

    bufMan.deleteFromTableWithCheckFunc("testTable", [](char * buf)
                                                        {
                                                            return (*buf == 0);
                                                        });

    cout << "after deletino: \n" ;
    printData(bufMan.queryCompleteTable("testTable"), 60);

    // cout << bufMan.queryTableWithOffset("testTable", 5004) << endl;
    // bufMan.writeBackAll();

    // bufMan.reloadTable("testTable");

    // TableInfo t2("table2", "indexLine",
    //             {Attribute("indexLine", "float", true)});

    // bufMan.createBufferForTable(t);
    // bufMan.createBufferForTable(t2);
    //
    // cout << bufMan.writeBackAll();


    // BufferPage p("testTable", 12, 0);
    // p.reloadPage();

    return 0;
}

// things to do: delete with comparison function
