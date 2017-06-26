#include "API.h"

API::API()
{
    cout << "Initializing... ";
    tableMap = catMan.initializeTables();
    for (auto & tableIter: tableMap)
    {
        recMan.initTable(tableIter.second);
    }

    indexMap = catMan.initializeIndexes();

    // to do : create index.

    cout << "Done. " << endl;
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
        cout << "Failed to drop table: Table " << tableName << " doe not exist! " << endl;
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

}

void API::deleteIndex(string indexName)
{

}


void API::insertInto(string table, Record newRecord)
{
    if (tableMap.find(table) == tableMap.end())
    {
        cout << "Error at insert : Table " << table << " does not exist! " << endl;
        return;
    }

    // generate queryes for unique attributes
    int attrIter = 0;
    for(auto & attr : tableMap[table].attributes)
    {
        if (attr.isUnique)
        {
            QueryBase * uniQuery;
            // vector<QueryBase*> uniQueryVec ({uniQuery});
            switch (attr.type)
            {
                case INT:
                    uniQuery = new SingleQuery<int>(attr.name, newRecord.elements[attrIter] -> intData);
                    break;
                case FLOAT:
                    uniQuery = new SingleQuery<float>(attr.name, newRecord.elements[attrIter] -> floatData);
                    break;
                case STRING:
                    uniQuery = new SingleQuery<string>(attr.name, newRecord.elements[attrIter] -> stringData);
                    break;
            }

            if (recMan.queryWithCondition(tableMap[table], vector<QueryBase *> ({uniQuery})).size() > 0)
            {
                cout << "Error at insert : value of unique attribute " << attr.name << " repeated. " << endl;
                return;
            }
        }
        attrIter ++ ;
    }
    if(recMan.insert(newRecord) >= 0)
    {
        cout << "Successfully inserted to table " << table << endl;
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
    cout << "deleted " << recMan.deleteWithCondition(tableMap[table], Querys) << " rows." << endl;
}

void API::selectFrom(string table, vector<string> columns, vector<QueryBase*> Querys)
{
    if (tableMap.find(table) == tableMap.end())
    {
        cout << "Error at select : Table " << table << " does not exist! " << endl;
        return;
    }

    // === print columns and get print vector ====
    vector<int> printIndex;

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
    cout << "------------------------------------------" << endl;
    cout << "| ";
    for(auto & col : columns)
    {
        cout << col << " | ";
    }
    cout << endl << "------------------------------------------" << endl;

    // ==== query and print result ===
    vector<Record> queryResult = recMan.queryWithCondition(tableMap[table], Querys);
    for (auto & record : queryResult)
    {
        cout << "| ";
        for (auto & index : printIndex)
        {
            cout << *record.elements[index] << " | ";
        }
        cout << endl;
    }
    cout << "------------------------------------------" << endl << endl;
}
