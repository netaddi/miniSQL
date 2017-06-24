#include "../src/RecordManager.h"
#include "../src/element.h"
#include "../src/debug.h"

int main(int argc, char const *argv[])
{
    RecordManager recMan;

    TableInfo t("testTable", "id",
                {Attribute("id", "int", true),
                 Attribute("float", "float", true),
                 Attribute("user", 8, true),
                 Attribute("password", 8, false)});

    vector<Element *> newItem({new Element(12),
                               new Element((float)123.456),
                               new Element("username"),
                               new Element("abcdefg")});
    Record r(t, newItem);
    recMan.initTable(t);
    recMan.insert(r);
    printData(recMan.bufMan.queryCompleteTable("testTable"), 24);

    Record r1 = recMan.queryWithOffset(t, {0})[0];
    cout << r1;

    // string s = "abcdefg";
    // ElementBase *e = new Element<dbString>(s);
    // printData((char *)s.c_str(), 10);
    // printData(e -> getBitToBuffer(), 10);
    return 0;
}
