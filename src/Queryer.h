#ifndef _QUERYER_H_
#define _QUERYER_H_

#include "stdafx.h"
// #include "RecordManager.h"
#include "element.h"
#include "Record.h"
//

class QueryBase
{
protected:
    string attrName;
public:
    QueryBase(string attr): attrName(attr){};
    virtual bool match(Record record) = 0;
};

template <class T>
class SingleQuery : public QueryBase
{
public:
    SingleQuery(string attrName, T data, bool unequal = false): QueryBase(attrName), targetData(data), matchUnequal(unequal){};
    T targetData;
    bool matchUnequal;
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

template <class T>
class infinityRangeQuery : public QueryBase
{
public:
    T seperateData;
    bool sepInlcluded;
    bool queryGreaterThan;

    infinityRangeQuery(string attrName, T seperateData, bool sepInlcluded, bool queryGreaterThan):
        QueryBase(attrName), seperateData(seperateData), sepInlcluded(sepInlcluded), queryGreaterThan(queryGreaterThan){};
    infinityRangeQuery(string attrName, T seperateData, string comparator):
        QueryBase(attrName), seperateData(seperateData)
        {
            if (comparator == ">")
            {
                sepInlcluded = false;
                queryGreaterThan = true;
            }
            if (comparator == "<")
            {
                sepInlcluded = false;
                queryGreaterThan = false;
            }
            if (comparator == ">=")
            {
                sepInlcluded = true;
                queryGreaterThan = true;
            }
            if (comparator == "<=")
            {
                sepInlcluded = true;
                queryGreaterThan = false;
            }
        };

    bool match(Record record);
};
#endif
