#include <string>
using std::string;
#include <vector>
using std::vector;

#ifndef _ELEMENT_H_
#define _ELEMENT_H_

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
    TableInfo(string tableName, string primaryKey, vector<Attribute> attrs):
        tableName(tableName), primaryKey(primaryKey), attributes(attrs){};
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
    int type;
    int length;

    Element(int i) :  intData(i), type(INT){};
    Element(float f) : floatData(f), type(FLOAT){};
    Element(string s, int length): stringData(s), length(length)
    {
        s.resize(length);
    };

};

bool operator<(const Element &v1, const Element &v2);
bool operator>(const Element &v1, const Element &v2);
bool operator>=(const Element &v1, const Element &v2);
bool operator<=(const Element &v1, const Element &v2);
bool operator==(const Element &v1, const Element &v2);
bool operator!=(const Element &v1, const Element &v2);

#endif
