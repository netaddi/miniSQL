#include "API.h"

API::API()
{
    cout << "Initializing... ";
    tableMap = catMan.initializeTables();
    indexMap = catMan.initializeIndexes();

    for (auto & tableIter: tableMap)
    {
        recMan.initTable(tableIter.second);
    }

    for (auto & indexIter : indexMap)
    {
        idxMan.createIndex(indexIter.second);
        vector<Record> allRecords = recMan.queryWithCondition(tableMap[indexIter.second.indexName], {});
        int offset = 0;
        for_each(allRecords.begin(), allRecords.end(), [&](auto record)
        {
           idxMan.insertIntoIndex(indexIter.second, record[indexIter.second.attributeName], offset);
           offset += tableMap[indexIter.second.indexName].recordLength;
        });
    }
    cout << "Done. " << endl;
}

void API::writeBackAll()
{
    recMan.writeBackAll();
}

void API::createTable(TableInfo newTable)
{
    if (tableMap.find(newTable.tableName) != tableMap.end())
    {
        cout << "Failed to create table: Table " << newTable.tableName << " already existed! " << endl;
        return;
    }
    if( catMan.createTableCatalog(newTable) )
    {
        // to do : create index for primary key.
        tableMap.insert(pair<string, TableInfo>(newTable.tableName, newTable));
        recMan.initTable(newTable);
        idxMan.createIndex(IndexInfo(newTable.tableName, newTable.primaryKey, catMan.getPrimaryKeyIndexName(newTable)));
        cout << "Successfully created table " << newTable.tableName << endl;
        return;
    }
    cout << "Creating table " << newTable.tableName << " failed. " << endl;
    return;
}

void API::deleteTable(string tableName)
{
    if (tableMap.find(tableName) == tableMap.end())
    {
        cout << "Failed to drop table: Table " << tableName << " does not exist! " << endl;
        return;
    }
    if( catMan.dropTableCatalog(tableName) )
    {
        // to do : drop index for primary key.
        recMan.dropTable(tableName);
        tableMap.erase(tableName);
        cout << "Successfully droped table " << tableName << endl;
        return;
    }
    cout << "Failed to drop table " << tableName << " failed. " << endl;
    return;
}


void API::createIndex(IndexInfo newIndex)
{
    if (tableMap.find(newIndex.tableName) == tableMap.end())
    {
        cout << "Failed to create index: Table " << newIndex.tableName << " does not exist! " << endl;
        return;
    }
    if (indexMap.find(newIndex.indexName) != indexMap.end())
    {
        cout << "Failed to create index: Index " << newIndex.indexName << " already existed! " << endl;
        return;
    }
    if (catMan.createIndexCatalog(newIndex))
    {
        // to do : finish index build.
        indexMap.insert(pair<string, IndexInfo>(newIndex.indexName, newIndex));
        cout << "Successfully created index " << newIndex.indexName << endl;
        return;
    }
    cout << "Creating index " << newIndex.indexName << " failed. " << endl;
    return;
}

void API::deleteIndex(string indexName)
{
    if (indexMap.find(indexName) == indexMap.end())
    {
        cout << "Failed to drop index: Index " << indexName << " does not exist! " << endl;
        return;
    }
    if( catMan.dropIndexCatalog(indexName) )
    {
        // to do : drop index.
        indexMap.erase(indexName);
        cout << "Successfully droped index " << indexName << endl;
        return;
    }
    cout << "Failed to drop index " << indexName << ". " << endl;
    return;
}


void API::insertInto(string table, vector<Element *> elements)
{
    if (tableMap.find(table) == tableMap.end())
    {
        cout << "Error at insert : Table " << table << " does not exist! " << endl;
        return;
    }
    auto attributes = tableMap[table].attributes;
    // for (size_t elementIter = 0; elementIter < attributes.size(); elementIter++)
    // {
    //     elements[elementIter] -> reinterpret(attributes[elementIter].type);
    // }

    // generate queryes for unique attributes
    int attrIter = 0;
    for(auto & attr : attributes)
    {
        elements[attrIter] -> reinterpret(attr.type);
        if (attr.isUnique)
        {
            QueryBase * uniQuery;
            // vector<QueryBase*> uniQueryVec ({uniQuery});
            switch (attr.type)
            {
                case INT:
                    uniQuery = new SingleQuery<int>(attr.name, elements[attrIter] -> intData);
                    break;
                case FLOAT:
                    uniQuery = new SingleQuery<float>(attr.name, elements[attrIter] -> floatData);
                    break;
                // case STRING:
                default:
                    uniQuery = new SingleQuery<string>(attr.name, elements[attrIter] -> stringData);
                    break;
            }

            if (recMan.queryWithCondition(tableMap[table], vector<QueryBase *> ({uniQuery})).size() > 0)
            {
                #if !PRS_TEST_MODE
                    cout << "Error at insert : value of unique attribute " << attr.name << " repeated. " << endl;
                #endif
                return;
            }
        }
        attrIter ++ ;
    }
    Record newRecord(tableMap[table], elements);
    int offset = recMan.insert(newRecord);
    if(offset >= 0)
    {
        for (auto & indexIter : indexMap)
        {
            if (indexIter.second.tableName == table)
            {
                idxMan.insertIntoIndex(indexIter.second, newRecord[indexIter.second.attributeName], offset);
            }
        }
        #if !PRS_TEST_MODE
            cout << "Successfully inserted to table " << table << endl;
        #endif
        return ;
    }
    cout << "Failed to insert into table " << table << endl;

}

void API::deleteFrom(string table, vector<QueryBase*> Querys)
{
    if (tableMap.find(table) == tableMap.end())
    {
        cout << "Error at select : Table " << table << " does not exist! " << endl;
        return;
    }
    if (recMan.deleteWithCondition(tableMap[table], Querys))
    {
        cout << "Successfully performed deletion. " << '\n';
        return ;
    }

    cout << "No record deleted.";

}

void API::selectFrom(string table, vector<string> columns, vector<QueryBase*> Querys)
{
    if (tableMap.find(table) == tableMap.end())
    {
        cout << "Error at select : Table " << table << " does not exist! " << endl;
        return;
    }

    // === print columns and get print vector ====
    // process * situation
    vector<int> printIndex;
    if (columns[0] == "*")
    {
        columns.clear();
        for (auto & attr : tableMap[table].attributes)
        {
            columns.push_back(attr.name);
        }
    }
    for (auto &col : columns)
    {
        size_t i;
        for (i = 0; i < tableMap[table].attributes.size(); i++)
        {
            if (tableMap[table].attributes[i].name == col)
            {
                printIndex.push_back(i);
                break;
            }
        }
        if (i == tableMap[table].attributes.size())
        {
            cout << "Error : Attribute " << col << " does not exist in table " << table << endl;
            return;
        }
    }
    cout << "-------------------------------------------------------" << endl;
    cout << "| ";
    for(auto & col : columns)
    {
        cout << col << " | ";
    }
    cout << endl << "-------------------------------------------------------" << endl;

    // ==== query and print result ===
    vector<Record> queryResult = recMan.queryWithCondition(tableMap[table], Querys);
    for (auto & record : queryResult)
    {
        if (record.validate())
        {
            cout << "| ";
            for (auto & index : printIndex)
            {
                cout << *record.elements[index] << " | ";
            }
            cout << endl;
        }
    }
    cout << "-------------------------------------------------------" << endl << endl;
}
int API::getAttributeType(string table, string attr)
{
    auto attrs = tableMap[table].attributes;
    for (auto & thisAttr : attrs )
    {
        if (thisAttr.name == attr)
        {
            return thisAttr.type;
        }
    }
    return -1;
}
int API::getAttributeSize(string table, string attr)
{
    auto attrs = tableMap[table].attributes;
    for (auto & thisAttr : attrs )
    {
        if (thisAttr.name == attr)
        {
            return thisAttr.size;
        }
    }
    return -1;
}
