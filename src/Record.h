
#include "stdafx.h"
#include "element.h"

#ifndef _RECORD_H_
#define _RECORD_H_
// #include "Queryer.h"


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
    void addElement(Element * e);
    friend ostream& operator<< (ostream& os, Record & r);

};


#endif
