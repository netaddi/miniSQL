#include <iostream>
#include "../src/CatalogManager.h"
#include "../src/element.h"

int main(int argc, char const *argv[]) {
    CatalogManager catMan;

    std::cout << "catMan.checkTableExistance(\"testTable\")"
              << catMan.checkTableExistance("testTable") << '\n';
    TableInfo t("testTable", "id",
                {Attribute("id", "int", true),
                 Attribute("user", 32, true),
                 Attribute("password", 32, false)});

    IndexInfo i("testTable", "user", "testIndex");
    // std::cout << "catMan.checkTableExistance(\"testTable\")"
    //           << catMan.checkTableExistance("testTable") << '\n';

    std::cout << catMan.createTableCatalog(t) << std::endl;
    std::cout << catMan.createIndexCatalog(i) << std::endl;

    std::cout << catMan.checkAttributeUnique("testTable", "id")
              << catMan.checkAttributeUnique("testTable", "user")
              << catMan.checkAttributeUnique("testTable", "password")
              << std::endl;
    IndexInfo temp_i = catMan.getIndexInfo("testTable", "id");
    IndexInfo i2 = catMan.getIndexInfo("testIndex");

    std::cout << temp_i.indexName << " "
         << temp_i.tableName << " "
         << temp_i.attributeName << std::endl;

    std::cout << i2.indexName << " "
      << i2.tableName << " "
      << i2.attributeName << std::endl;
    std::cout << "testIndex.exist:" << catMan.checkIndexExistanceWithName("testIndex") << '\n';
    std::cout << catMan.dropIndexCatalog("testIndex") << '\n';
    std::cout << "testIndex.exist:" << catMan.checkIndexExistanceWithName("testIndex") << '\n';
    std::cout << catMan.dropTableCatalog("testTable");
    // catMan.dropTableCatalog("testTable");
    // std::cout << "catMan.checkTableExistance(\"testTable\")"
    //           << catMan.checkTableExistance("testTable") << '\n';
    //
    // TableInfo t2("table2", "indexLine",
    //             {Attribute("indexLine", "float", true)});
    // std::cout << catMan.createTableCatalog(t2) << std::endl;
    //
    // TableInfo temp = catMan.getTableInfo("testTable");
    // temp.tableName = "readTable";
    //
    // std::cout << catMan.createTableCatalog(temp) << std::endl;



    std::cout << "all test done.\n";
    return 0;
}
