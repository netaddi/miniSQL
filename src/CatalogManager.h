// author: netaddi
// email:  prof.weismann1@gmail.com
// site:   young.moe
//====================================
// date created: 30/05/2017
#include <string>
using std::string;
#include <vector>
using std::vector;
#include "__API.h"

#ifndef _CATALOGMANAGER_H_
#define _CATALOGMANAGER_H_


class CatalogManager
{
private:
    string categoryDir = "data/catalog/";
    string tableInfoFilename = "table.cat";
    string indexInfoCatalog = "index.cat";
public:
    CatalogManager();
    ~CatalogManager();

    // return true if success
    bool createTableCatalog(TableInfo tableInfo);
    bool dropTableCatalog(string tableName);
    bool createIndexCatalog(IndexInfo indexInfo);
    bool dropIndexCatalog(string indexName);

    bool checkTableExistance(string tableName);
    bool checkIndexExistanceWithName(string indexName);
    bool checkIndexExistanceWithAttr(string tableName, string attributeName);
    bool checkAttributeUnique(string tableName, string attributeName);

    TableInfo getTableInfo(string tableName);
    IndexInfo getIndexInfo(string indexName);
};

#endif