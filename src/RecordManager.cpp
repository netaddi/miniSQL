#include "RecordManager.h"
#include "stdafx.h"
// #include "element.h"


void Record::addElement(Element * e)
{
    elements.push_back(e);
}

ostream & operator<< (ostream& os, Record & r)
{
    os << "| ";

    // for (const auto & element : r.elements)
    // {
    //     os << *element << " | ";
    // }

    for_each(r.elements.begin(), r.elements.end(), [&](auto e)
                                                    {
                                                        os << *e << "  | ";
                                                    });

    os << endl;
    return os;
}

RecordManager::RecordManager()
{

}

void RecordManager::dropTable(string tableName)
{
    bufMan.dropTableBuffer(tableName);
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
    vector<Record> result;
    char * totalBuffer = bufMan.queryCompleteTable(table.tableName);
    int totalBufferSize = bufMan.getTableBufferSize(table.tableName);
    for (int bufferIter = 0;  bufferIter < totalBufferSize;  bufferIter+= table.recordLength)
    {
        int elementPtr = 0;
        vector<Element * > elementsInRecord;
        // for (auto & attr : table.attributes )
        // {
        //     Element * tempElement = new Element(attr.type, attr.size, totalBuffer + bufferIter + elementPtr);
        // }
        for_each(table.attributes.begin(), table.attributes.end(), [&](auto attr)
                {
                    elementsInRecord.push_back(new Element(attr.type, attr.size, totalBuffer + bufferIter + elementPtr));
                    elementPtr += attr.size;
                });
        Record thisRecord(table, elementsInRecord);
        bool matched = accumulate(querys.begin(), querys.end(), true, [&](bool matched, auto query)
                                                                            {
                                                                                #if DBG
                                                                                    cout << "checking " << thisRecord << "result : " << query -> match(thisRecord) << endl;
                                                                                #endif
                                                                                return matched && query -> match(thisRecord);
                                                                            });
        if (matched)
        {
            result.push_back(thisRecord);
        }
    }
    return result;
}

bool RecordManager::deleteWithOffset(string table, vector<int> offsets)
{
    return accumulate(offsets.begin(), offsets.end(), true, [=](bool success, int offset)
            {
                return success && bufMan.deleteFromTableWithOffset(table, offset);
            });
    // for_each(offsets.begin(), offsets.end(), [](auto offset)
    //                                             {
    //
    //                                             });
    // return bufMan -> deleteFromTableWithOffset(table, );
}


int RecordManager::deleteWithCondition(TableInfo table, vector<QueryBase *> querys)
{
    int totalBufferSize = bufMan.getTableBufferSize(table.tableName);
    int deleteCount = 0;
    for (int bufferIter = 0;  bufferIter < totalBufferSize;  bufferIter+= table.recordLength)
    {
        bufMan.deleteFromTableWithCheckFunc(table.tableName, [&](char * recordBuffer)
        {
            int elementPtr = 0;
            vector<Element * > elementsInRecord;
            for_each(table.attributes.begin(), table.attributes.end(), [&](auto attr)
                {
                    elementsInRecord.push_back(new Element(attr.type, attr.size, recordBuffer + elementPtr));
                    elementPtr += attr.size;
                });
            Record tempRecord(table, elementsInRecord);
            if( accumulate(querys.begin(), querys.end(), true, [&](bool matched, auto query)
                {
                    return matched && query -> match(tempRecord);
                }) )
            {
                deleteCount ++ ;
                return true;
            }
            return false;
        });
    }
    return deleteCount;
}

void RecordManager::writeBackAll()
{
    bufMan.writeBackAll();
}
