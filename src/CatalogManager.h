// author: netaddi
// email:  prof.weismann1@gmail.com
// site:   young.moe
//====================================
// date created: 30/05/2017
#include <string>
using std::string;
#include <vector>
using std::vector;
#include "element.h"

#ifndef _CATALOGMANAGER_H_
#define _CATALOGMANAGER_H_


class CatalogManager
{
private:
    string tableCatalogDir = "data/catalog/";
    // string indexCatalogDir = "data/catalog/index/";
    string indexDir;
    string tableInfoFilename = "data/table.cat";
    // string tempTableInfoFilename = "data/table.tmp";
    string indexInfoCatalog = "data/index.cat";

    bool dropLineFromFile(string filename, string element, int elementCount);
    bool checkLineExistance(string filename, string lineToCheck);

public:
    // CatalogManager();
    // ~CatalogManager();

    string getPrimaryKeyIndexName(TableInfo table);
    // return true if success
    bool createTableCatalog(TableInfo tableInfo);
    bool dropTableCatalog(string tableName);
    bool createIndexCatalog(IndexInfo indexInfo);
    bool dropIndexCatalog(string indexName);
    bool dropIndexOfTable(string tableName);

    bool checkTableExistance(string tableName);
    bool checkIndexExistanceWithName(string indexName);
    bool checkIndexExistanceWithAttr(string tableName, string attributeName);
    bool checkAttributeUnique(string tableName, string attributeName);

    TableInfo getTableInfo(string tableName);
    IndexInfo getIndexInfo(string indexName);
    IndexInfo getIndexInfo(string tableName, string attributeName);

    map<string, TableInfo> initializeTables();
    map<string, IndexInfo> initializeIndexes();

};

#endif
