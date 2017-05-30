#include <string>
using std::string;
#include <vector>
using std::vector;

#ifndef _ELEMENT_H_
#define _ELEMENT_H_

class Attribute
{
public:
    string type;
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
            type = "int";
            size = sizeof(int);
        }
        if (typeName.length() == 5)
        {
            type = "float";
            size = sizeof(float);
        }
    };
    Attribute(string attrName, int length, bool unique)
    {
        isUnique = unique;
        name = attrName;
        type = "string";
        size = length;
    };
};

class TableInfo
{
public:
    string tableName;
    string primaryKey;
    int attributeCount;
    vector<Attribute> attributes;
};

class IndexInfo
{
public:
    string indexName;
    string tableName;
    // Attribute attribute;
    string attributeName;

    IndexInfo(string table, string attribute, string index):indexName(index), tableName(table), attributeName(attribute){};
    IndexInfo(string table, string attribute):tableName(table), attributeName(attribute){};
    IndexInfo(string index): indexName(index){};
};

class Element
{
public:
    int intData;
    float floatData;
    string stringData;
    string type;

};

#endif
