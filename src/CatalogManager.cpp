#include "CatalogManager.h"
#include <fstream>
#include <iostream>
#include <sstream>
using std::ofstream;
using std::ifstream;
using std::istringstream;
using std::endl;
using std::cout;


string CatalogManager::getPrimaryKeyIndexName(TableInfo table)
{
    return "___" + table.tableName + "__PRIK_INDX____";
}

bool CatalogManager::createTableCatalog(TableInfo tableInfo)
{
    ofstream catalogFile;
    // cout << tableCatalogDir + tableInfo.tableName << endl;
    catalogFile.open(tableCatalogDir + tableInfo.tableName);
    catalogFile << tableInfo.tableName << endl;
    catalogFile << tableInfo.primaryKey << endl;
    catalogFile << tableInfo.attributes.size() << endl;
    for ( const auto & attr : tableInfo.attributes )
    {
        catalogFile << attr.name << '\t'
                    << attr.type << '\t'
                    << attr.size << '\t'
                    << (attr.isUnique ? '1' : '0') << endl;
    }
    catalogFile.close();

    ofstream tableInfoFile;
    tableInfoFile.open(tableInfoFilename, std::ios_base::app);
    tableInfoFile << tableInfo.tableName << endl;
    tableInfoFile.close();

    IndexInfo primaryIndexForTable(tableInfo.tableName, tableInfo.primaryKey,
        getPrimaryKeyIndexName(tableInfo));

    return (catalogFile.good()
        && tableInfoFile.good()
        && createIndexCatalog(primaryIndexForTable));
}

// bool CatalogManager::dropElementLineFromFile()

bool CatalogManager::dropLineFromFile(string filename, string element, int elementCount)
{
    string line;
    string tempFileName = filename + ".tmp";
    ifstream mainFile;
    ofstream tempFile;

    mainFile.open(filename);
    tempFile.open(tempFileName);

    while (getline(mainFile, line))
    {
        istringstream lineStringStream(line);
        string StrToCmp;
        for (int i = 0; i < elementCount; i++)
        {
            lineStringStream >> StrToCmp;
        }
        if (StrToCmp != element)
        {
            tempFile << line << endl;
        }
    }
    tempFile.close();
    mainFile.close();

    remove(filename.c_str());
    rename(tempFileName.c_str(), filename.c_str());
    // cout << "debug" << mainFile.good() << tempFile.good();
    return (tempFile.good());
}

bool CatalogManager::dropTableCatalog(string tableName)
{
    return dropLineFromFile(tableInfoFilename, tableName, 1)
        && (!remove((tableCatalogDir + tableName).c_str()))
        && dropIndexOfTable(tableName);
}

bool CatalogManager::createIndexCatalog(IndexInfo indexInfo)
{
    ofstream indexInfoCatalogFile;
    indexInfoCatalogFile.open(indexInfoCatalog, std::ios_base::app);
    indexInfoCatalogFile << indexInfo.indexName << " "
                         << indexInfo.tableName << " "
                         << indexInfo.attributeName << endl;
    indexInfoCatalogFile.close();
    return (!indexInfoCatalogFile.fail());
}

bool CatalogManager::dropIndexCatalog(string indexName)
{
    return dropLineFromFile(indexInfoCatalog, indexName, 1);
}
bool CatalogManager::dropIndexOfTable(string tableName)
{
    return dropLineFromFile(indexInfoCatalog, tableName, 2);
}

bool CatalogManager::checkLineExistance(string filename, string lineToCheck)
{
    ifstream fileToCheck;
    fileToCheck.open(filename);
    // cout << filename << endl;
    string line;
    while(getline(fileToCheck, line))
    {
        cout << line << "  " << lineToCheck << '\n';
        if (line == lineToCheck)
        {
            return true;
        }
    }
    return false;
}

bool CatalogManager::checkTableExistance(string tableName)
{
    return checkLineExistance(tableInfoFilename, tableName);
}


IndexInfo CatalogManager::getIndexInfo(string indexName)
{
    string indexLine;
    string _tableName, _indexName, _attrName;
    ifstream indexInfoCatalogFile;
    indexInfoCatalogFile.open(indexInfoCatalog);
    while(getline(indexInfoCatalogFile, indexLine))
    {
        istringstream indexLineStream(indexLine);
        indexLineStream >> _indexName >> _tableName >> _attrName;
        if (indexName == _indexName)
        {
            return IndexInfo(_tableName, _attrName, _indexName);
        }
    }
    return IndexInfo("");
    // this IndexInfo obj not available.
}

IndexInfo CatalogManager::getIndexInfo(string tableName, string attributeName)
{
    string indexLine;
    string _tableName, _indexName, _attrName;
    ifstream indexInfoCatalogFile;
    indexInfoCatalogFile.open(indexInfoCatalog);
    while(getline(indexInfoCatalogFile, indexLine))
    {
        istringstream indexLineStream(indexLine);
        indexLineStream >> _indexName >> _tableName >> _attrName;
        if (tableName == _tableName && attributeName == _attrName)
        {
            return IndexInfo(_tableName, _attrName, _indexName);
        }
    }
    return IndexInfo("");
    // this IndexInfo obj not available.
}

bool CatalogManager::checkIndexExistanceWithName(string indexName)
{
    return (getIndexInfo(indexName).isValid());
}

bool CatalogManager::checkIndexExistanceWithAttr(string tableName, string attributeName)
{
    return (getIndexInfo(tableName, attributeName).isValid());
}

bool CatalogManager::checkAttributeUnique(string tableName, string attributeName)
{
    TableInfo tableInfo = getTableInfo(tableName);
    for ( const auto & attr : tableInfo.attributes )
    {
        if (attr.name == attributeName)
        {
            return attr.isUnique;
        }
    }
    return false;
}


TableInfo CatalogManager::getTableInfo(string tableName)
{
    string _table, _primaryKey, _attr;
    int attrCount, _type, _size;
    bool _unique;
    vector<Attribute> _attributes;

    ifstream catalogFile;
    catalogFile.open(tableCatalogDir + tableName);
    catalogFile >> _table >> _primaryKey >> attrCount;
    for (int i = 0; i < attrCount; i++)
    {
        catalogFile >> _attr >> _type >> _size >> _unique;
        _attributes.push_back(Attribute(_attr, _type, _size, _unique));
    }
    TableInfo returnTable(_table, _primaryKey, _attributes);
    return returnTable;
}
