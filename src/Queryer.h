#ifndef _QUERYER_H_
#define _QUERYER_H_

#include "stdafx.h"
// #include "RecordManager.h"
#include "element.h"
#include "Record.h"

class QueryBase
{
protected:
    string attrName;
public:
    QueryBase(string attr): attrName(attr){};
    virtual bool match() = 0;
};

template <class T>
class SingleQuery : QueryBase
{
public:
    SingleQuery(string attrName, T data): QueryBase(attrName), targetData(data){};
    T targetData;
    bool match(Record record);
};

template <class T>
class RangeQuery : public QueryBase
{
public:
    T minData;
    T maxData;
    bool minIncluded;
    bool maxIncluded;

    RangeQuery(string attrName, T minData, T maxData, bool minInc, bool maxInc): QueryBase(attrName),
        minData(minData), maxData(maxData), minIncluded(minInc), maxIncluded(maxInc){};

    bool match(Record record);

};

#endif
