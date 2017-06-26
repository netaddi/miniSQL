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
                               new Element("123a"),
                               new Element("abcdefg")});

    vector<Element *> newItem1({new Element(10),
                              new Element((float)456.789),
                              new Element("username"),
                              new Element("abcdefg")});

    vector<Element *> newItem2({new Element(8),
                            new Element((float)789.123),
                            new Element("username"),
                            new Element("abcdefg")});

    Record r(t, newItem);
    Record r1(t, newItem1);
    Record r2(t, newItem2);
    recMan.initTable(t);
    recMan.insert(r);
    recMan.insert(r1);
    recMan.insert(r2);
    printData(recMan.bufMan.queryCompleteTable("testTable"), 24);

    SingleQuery<int> s("id", 6, true);
    RangeQuery<float> rq("float", 23.567, 1000, false, false);
    infinityRangeQuery<int> irq("id", 10, true, true);
    cout << s.match(r) << s.match(r1) << s.match(r2) << endl;
    cout << rq.match(r) << rq.match(r1) << rq.match(r2) << endl;

    vector<QueryBase *> queryVec({&irq});
    // queryVec.push_back(&s);
    cout << recMan.queryWithCondition(t, queryVec)[0] << endl;
    // cout << recMan.queryWithCondition(t, queryVec)[1] << endl;

    // recMan.deleteWithOffset(t.tableName, {0});
    recMan.deleteWithCondition(t, queryVec);
    queryVec[0] = &s;
    cout << recMan.queryWithCondition(t, queryVec).size() << endl;

    // ==================================
    // test for large scale insertion
    // ==================================
    // for (size_t i = 0; i < 100000; i++)
    // {
    //     vector<Element *> newItem0({new Element((int)i),
    //                                 new Element(((float)i) / (float)2),
    //                                 new Element("size"),
    //                                 new Element("str")});
    //     Record r0(t, newItem0);
    //     recMan.insert(r0);
    //     // cout << i << " inserted" << endl;
    // }



    // Record rs = recMan.queryWithOffset(t, {0})[0];
    // cout << rs;

    // string s = "abcdefg";
    // ElementBase *e = new Element<dbString>(s);
    // printData((char *)s.c_str(), 10);
    // printData(e -> getBitToBuffer(), 10);
    return 0;
}
