#include "../src/API.h"

int main(int argc, char const *argv[])
{
    API dbAPI;
    // ==================================
    // test CREATE TABLE
    // ==================================
    cout << "============ table operation test ==============" << endl;
    TableInfo t("student", "id",
            {Attribute("id", INT, sizeof(int), true),
             Attribute("float", FLOAT, sizeof(float), true),
             Attribute("name", STRING, 32, false)
            });


    cout << "----result of create table testTable: " << endl;
    dbAPI.createTable(t);
    cout << "----create same table again: " << endl;
    dbAPI.createTable(t);
    cout << "----test drop table" << endl;
    dbAPI.deleteTable(t.tableName);
    cout << "----insert table again: " << endl;
    dbAPI.createTable(t);


    // ======================================
    //  test insert
    // ======================================
    cout << endl << "============ insert test ==============" << endl;
    cout << "----insert new data" << endl;
    vector<Element *> newItem({new Element(12),
                               new Element((float)123.456),
                               new Element("123a1111"),
                               new Element("abcdefg")});
    Record r(t, newItem);
    dbAPI.insertInto(t.tableName, r);
    cout << "----insert new data 2 " << endl;
    vector<Element *> newItem1({new Element(10),
                              new Element((float)123.46),
                              new Element("123a"),
                              new Element("abcdef")});
    Record r1(t, newItem1);
    dbAPI.insertInto(t.tableName, r1);
    cout << "----insert repeated data " << endl;
    dbAPI.insertInto(t.tableName, r);

    // ======================================
    //  test select
    // ======================================
    cout << endl << "============ select test ==============" << endl;
    cout << "----select " << endl;
    SingleQuery<int> s("id", 10, true);
    RangeQuery<float> rq("float", 23.567, 1000, false, false);
    infinityRangeQuery<int> irq("id", 10, true, true);
    vector<QueryBase *> queryVec({&rq});
    dbAPI.selectFrom(string("testTable"), vector<string>({"float", "id", "user"}), queryVec);

    // ======================================
    //  test delete
    // ======================================
    cout << endl << "============ delete test ==============" << endl;
    vector<QueryBase *> deleteQuery({&s});
    cout << "----perform delete" << endl;
    dbAPI.deleteFrom("testTable", deleteQuery);
    // print after delete
    cout << "----select after delete" << endl;
    dbAPI.selectFrom(string("testTable"), vector<string>({"float", "id", "user"}), queryVec);


    return 0;
}
