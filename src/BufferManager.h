// #pragma once


#ifndef _BUFFER_MANAGER_H_
#define _BUFFER_MANAGER_H_


#include "stdafx.h"
#include "element.h"
#include "debug.h"

#define PAGE_SIZE (int)(4096 - sizeof(int))
// #define PAGE_SIZE 22


// class for single buffer page
class BufferPage
{
private:
    string getPageFileName();
public:
    bool dirty;
    // bool full;
    string table;
    int pageId;
    string pageFileName;
    int recordLength;
    int usedLength;
    // int availableLength;
    char data[PAGE_SIZE];

    BufferPage(string table, int recordLength, int pageId):
        dirty(true),
        table(table), pageId(pageId), pageFileName(getPageFileName()),
        recordLength(recordLength), usedLength(0){};

    inline bool isFull();
    bool reloadPage();
    bool writeBackPage();
    void dropPage();

    char * readRecordWithOffset(int offset);

    bool deleteWithOffset(int offset);
    bool deleteWithCheckFunc(function<bool (char * )> checkDeletionFunction);

    //return offset
    int insertDataToPage(char * newData);
};

// class for buffers for each table
class BufferPool
{
private:
    inline string getPageFileName(int id);
public:
    string table;
    int recordLength;
    bool dirty;
    // int bufferCount;
    int activePageId;
    int validPageLength;
    vector<BufferPage *> buffers;
    vector<bool> pageFull;

    BufferPool (TableInfo & table);
    // :
    //     table(table.tableName), recordLength(table.recordLength), dirty(true), bufferCount(0), activePageId(-1){};

    void addNewBufferToPool();
    void dropBufferPool();

    bool reloadBuffer();
    bool writeBackBuffers();

    void findactivePageId();
    int insertData(char * newData);

    int getTotalBufferSize();

    char * queryAllData();
    char * queryWithOffset(int offset);

    bool deleteWithOffset(int offset);
    bool deleteWithCheckFunc(function<bool (char * )> checkDeletionFunction);
};

class BufferManager
{
public:
    // @key : table name
    // @value : buffer pool for that table
    map<string, BufferPool *> bufferMap;
// public:

    void createBufferForTable(TableInfo & table);
    void dropTableBuffer(string tableName);

    bool writeBackAll();
    bool writeBackTable(string table);
    bool reloadTable(string table);

    // return offset of the inserted data
    int insertIntoTable(string table, char * newData);
    char * queryTableWithOffset(string table, int offset);
    char * queryCompleteTable(string table);
    int getTableBufferSize(string table);

    bool deleteFromTableWithOffset(string table, int offset);
    bool deleteFromTableWithCheckFunc(string table, function<bool (char *)> checkDeletionFunction);
};

#endif
