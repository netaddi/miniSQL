#include "RecordManager.h"
#include "stdafx.h"
// #include "element.h"

ostream & operator<< (ostream& os, Record & r)
{
    os << "| ";

    for (const auto & element : r.elements)
    {
        os << *element << " | ";
    }

    // for_each(r.elements.begin(), r.elements.end(), [=](Element * e)
    //                                                 {
    //                                                     os << e << "  |";
    //                                                 })

    os << endl;
    return os;
}

RecordManager::RecordManager()
{

}

void RecordManager::initTable(TableInfo table)
{
    bufMan.createBufferForTable(table);
}

int RecordManager::insert(Record record)
{
    char recordBuffer[record.recordLength];
    int bufferPointer = 0;

    for (size_t attributeIter = 0;
        attributeIter < record.table.attributes.size();
        attributeIter++)
    {
        char * dataBuffer =  record.elements[attributeIter] -> getBitToBuffer();
        for (int dataPointer = 0;
            dataPointer < record.table.attributes[attributeIter].size;
            dataPointer++)
        {
            recordBuffer[bufferPointer++] = dataBuffer[dataPointer];
        }
    }

    return bufMan.insertIntoTable(record.table.tableName, (char *)recordBuffer);
}

vector<Record> RecordManager::queryWithOffset(TableInfo table, vector<int> offsets)
{
    vector<Record> results;
    char * binaryResult;
    for (auto & offset : offsets)
    {
        binaryResult = bufMan.queryTableWithOffset(table.tableName, offset);
        if (binaryResult)
        {
            Record retrivedRecord(table);
            int bufferPtr = 0;
            for(auto & attr : table.attributes)
            {
                retrivedRecord.addElement(new Element(attr.type, attr.size, binaryResult + bufferPtr));
                bufferPtr += attr.size;
            }
            results.push_back(retrivedRecord);
        }
    }
    return results;
}

vector<Record> RecordManager::queryWithCondition(TableInfo table, vector<QueryBase *> querys)
{

}

bool RecordManager::deleteWithOffset(string table, vector<int> offsets)
{

}

bool RecordManager::deleteWithCondition(string table, vector<QueryBase *> querys)
{

}
