#include "IndexManager.h"


void IndexManager::createIndex(IndexInfo indexInfo)
{
    if (trees.find(pair<string, string>(indexInfo.tableName, indexInfo.attributeName)) == trees.end())
    {
        trees[pair<string, string>(indexInfo.tableName, indexInfo.attributeName)] = BTree<Element, int>();
        // trees.insert(pair< pair<string, string> , BTree<Element, int> >(pair<string, string> (indexInfo.tableName, indexInfo.attributeName), BTree()));
    }
}

void IndexManager::dropIndex(IndexInfo indexInfo)
{
    if (trees.find(pair<string, string>(indexInfo.tableName, indexInfo.attributeName)) != trees.end())
    {
        trees.erase(pair<string, string>(indexInfo.tableName, indexInfo.attributeName));
    }
}

void IndexManager::dropAllIndexFromTable(string tableName)
{
    for (auto treeIter : trees)
    {
        if (treeIter.first.first == tableName)
        {
            trees.erase(treeIter.first);
        }
    }
}

void IndexManager::insertIntoIndex(IndexInfo index, Element value, int offset)
{
    if (trees.find(pair<string, string>(index.tableName, index.attributeName)) != trees.end())
    {
        trees[pair<string, string>(index.tableName, index.attributeName)].Insert(value, offset);
    }
}

int IndexManager::queryFromIndex(string table, string attr, Element * e)
{
    if (trees.find(pair<string, string>(table, attr)) != trees.end())
    {
        return trees[pair<string, string>(table, attr)][*e];
    }
    return -1;
}

void IndexManager::deleteFromIndex(IndexInfo index, Element value)
{
    if (trees.find(pair<string, string>(index.tableName, index.attributeName)) != trees.end())
    {
        trees[pair<string, string>(index.tableName, index.attributeName)].Delete(value);
        // trees[pair<string, string>(index.tableName, index.attributeName)].delete(value);
    }
}
