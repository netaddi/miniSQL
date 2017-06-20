#include <vector>
using std::vector;

#ifndef _BPLUSTREE_H_
#define _BPLUSTREE_H_

template <class T>
class Key
{
    T value;
    int offset;
};

template <class T>
class Query
{
    bool singleQuery;
    T lowerBound;
    T upperBound;
    bool leftIncluded;
    bool rightIncluded;
};

template <class T>
class BPlusTreeNode
{
public:
    vector< Key<T> > keys;
    int keyCount;
    BPlusTreeNode<T> * parent;
    vector< BPlusTreeNode<T>* > children;
    BPlusTreeNode<T> * next;

    BPlusTreeNode(): keyCount(0), parent(nullptr), next(nullptr){};
    BPlusTreeNode(Key<T> value) :
        keys(new vector< Key<T> >(value)),
        keyCount(1),
        parent(nullptr),
        children(new vector< BPlusTreeNode<T>* >(nullptr)){};
};

template <class T>
class BPlusTree
{
private:
    BPlusTreeNode<T> * root;
    int size;
    int minElementCount;
    int maxElementCount;

    bool searchKey(Key<T> value);
public:
    BPlusTree() {};
    ~BPlusTree() {};

    vector< BPlusTreeNode <T> *> query (Query<T> query);

    bool insertKey(Key<T> value);
    bool deleteKey(Key<T> value);

    void print();

};
template<class T>
bool BPlusTree<T>::insertKey(Key<T> value)
{
    if (!root)
    {
        root = new BPlusTreeNode<T>(value);
        return true;
    }


}

#endif
