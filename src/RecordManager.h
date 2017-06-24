#include "stdafx.h"
#include "BufferManager.h"

#ifndef _RECORD_MANAGER_H_
#define _RECORD_MANAGER_H_

class Record
{
public:
    TableInfo table;
    vector<Element * > elements;
    int recordLength;

    Record(TableInfo table, vector<Element *> elements):
        table(table), elements(elements), recordLength(table.recordLength){};
    Record(TableInfo table):
        table(table),                     recordLength(table.recordLength){};
    void addElement(Element * e)
    {
        elements.push_back(e);
    }
    friend ostream& operator<< (ostream& os, Record & r);
};

class RecordManager
{
private:
public:
    BufferManager bufMan;
    RecordManager();

    void initTable(TableInfo table);
    int insert(Record record);
    vector<Record> queryWithOffset(TableInfo table, vector<int> offsets);
    vector<Record> queryWithCondition(TableInfo table, vector<QueryBase *> querys);
    bool deleteWithOffset(string table, vector<int> offsets);
    bool deleteWithCondition(string table, vector<QueryBase *> querys);
};



#endif
