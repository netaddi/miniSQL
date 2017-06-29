

#ifndef _API_H_
#define _API_H_

#include "stdafx.h"
#include "element.h"
#include "Record.h"
#include "Queryer.h"
#include "BufferManager.h"
#include "CatalogManager.h"
#include "RecordManager.h"
#include "IndexManager.h"


class API
{
private:
    map<string, TableInfo> tableMap;
    map<string, IndexInfo> indexMap;

    RecordManager recMan;
    CatalogManager catMan;
    IndexManager idxMan;
public:
    API();

    void writeBackAll();

    void createTable(TableInfo newTable);
    void deleteTable(string tableName);

    void createIndex(IndexInfo newIndex);
    void deleteIndex(string indexName);

    void insertInto(string table, vector<Element *> elements);
    void deleteFrom(string table, vector<QueryBase*> Querys);
    void selectFrom(string table, vector<string> columns, vector<QueryBase*> Querys);
    int getAttributeType(string table, string attr);
    int getAttributeSize(string table, string attr);
};

#endif
