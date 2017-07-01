
#ifndef _INDEXMANAGER_H_
#define _INDEXMANAGER_H_
#include "element.h"
#include "stdafx.h"
#include "BPlusTree.h"

class IndexManager
{
public:
    // IndexManager();
    // ~IndexManager();

    map < pair<string, string> , BTree<Element, int> > trees;
    // vector < BTree<Element, int> > BPTrees;

    void createIndex(IndexInfo indexInfo);
    void dropIndex(IndexInfo indexInfo);
    void dropAllIndexFromTable(string tableName);
    void insertIntoIndex(IndexInfo index, Element value, int offset);
    int queryFromIndex(string table, string attr, Element * e);
    void deleteFromIndex(IndexInfo index, Element value);

    // vector<int> queryIndex(IndexInfo index, IndexQuery query);

};

#endif
