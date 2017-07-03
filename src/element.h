// #pragma once

#include "stdafx.h"
// #include "RecordManager.h"

#ifndef _ELEMENT_H_
#define _ELEMENT_H_

#define INT_SIZE    (int)sizeof(int)
#define FLOAT_SIZE  (int)sizeof(float)

#define INVALID_CHAR_DATA 127
#define INVALID_INT_DATA (int)0x7F7F7F7F

typedef char * dbString;

enum dataTypes { INT, FLOAT, STRING };

class Attribute
{
public:
    int type;
    string name;
    int size;
    bool isUnique;
    // this  for
    Attribute(string attrName, string typeName, bool unique)
    {
        name = attrName;
        isUnique = unique;
        if (typeName.length() == 3)
        {
            type = INT;
            size = sizeof(int);
        }
        if (typeName.length() == 5)
        {
            type = FLOAT;
            size = sizeof(float);
        }
    };
    Attribute(string attrName, int length, bool unique):
        type(STRING), name(attrName), size(length), isUnique(unique){};

    Attribute(string attrName, int type, int size, bool unique):
        type(type), name(attrName), size(size), isUnique(unique){};

};

class TableInfo
{
public:
    string tableName;
    string primaryKey;
    // int attributeCount;
    vector<Attribute> attributes;
    int recordLength;
    TableInfo(){};
    TableInfo(string tableName, string primaryKey, vector<Attribute> attrs):
        tableName(tableName), primaryKey(primaryKey), attributes(attrs),
        recordLength(accumulate(attrs.begin(), attrs.end(), 0,
                                [](int length, Attribute attr)
                                {
                                    return length + attr.size;
                                })) {};
    // TableInfo(string tableName, string primaryKey, int attrCnt, vector<Attribute> attrs):
    //     tableName(tableName), primaryKey(primaryKey), attributeCount(attrCnt), attributes(attrs){};
};

class IndexInfo
{
public:
    string indexName;
    string tableName;
    // Attribute attribute;
    string attributeName;
    int recordLength;

    IndexInfo(string table, string attribute, string index):
        indexName(index), tableName(table), attributeName(attribute){};
    IndexInfo(string table, string attribute):
        tableName(table), attributeName(attribute){};
    IndexInfo(string index): indexName(index){};
    bool isValid()
    {
        return (indexName != "");
    }
};

class Element
{
public:
    int intData;
    float floatData;
    string stringData;
    bool valid = true;

    int type;
    int size;

    Element(int type, int size, char * sourceBit): type(type), size(size)
    {
        retriveFromBit(sourceBit);
    };
    Element(){};
    Element(int data): intData(data), type(INT), size(sizeof(data)){};
    Element(float data): floatData(data), type(FLOAT), size(sizeof(data)){};
    Element(string data): stringData(data), type(STRING), size(data.size()){};
    void resize(int newSize)
    {
        size = newSize;
    }

    void reinterpret(int newType)
    {
        switch(newType)
        {
            case INT:
                type = INT;
                intData = stoi(stringData);
                size = sizeof(int);
                return;
            case FLOAT:
                type = FLOAT;
                floatData = stof(stringData);
                size = sizeof(float);
                return;
        }
    }

    char * getBitToBuffer()
    {
        switch (type)
        {
            case INT:
                return reinterpret_cast<char *> (&intData);
            case FLOAT:
                return reinterpret_cast<char *> (&floatData);
            case STRING:
                return (char *)stringData.c_str();
        }
        return nullptr;
    }

    void retriveFromBit(char * sourceBit)
    {
        if (*(int *)sourceBit == INVALID_INT_DATA)
        {
            valid = false;
            return;
        }
        switch(type)
        {
            case INT:
                copy(sourceBit, sourceBit + 4, reinterpret_cast<char *> (&intData));
                break;
            case FLOAT:
                copy(sourceBit, sourceBit + 4, reinterpret_cast<char *> (&floatData));
                break;
            case STRING:
                stringData = string(sourceBit);
                stringData.resize(size);
        }
    };

    friend ostream & operator << (ostream& os, const Element & e);

    friend bool operator < (const Element & lhs, const Element & rhs);
    friend bool operator > (const Element & lhs, const Element & rhs);
    friend bool operator <= (Element & lhs, Element & rhs);
    friend bool operator >= (Element & lhs, Element & rhs);
    friend bool operator == (const Element & lhs, const Element & rhs);
    friend bool operator != (Element & lhs, Element & rhs);
};



#endif
