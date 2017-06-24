// #pragma once

#include "stdafx.h"
#include "BufferManager.h"
#include "element.h"
#include "Record.h"
#include "Queryer.h"

#ifndef _RECORD_MANAGER_H_
#define _RECORD_MANAGER_H_



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
