#ifndef _BPLUSTREE_H_
#define _BPLUSTREE_H_
#include "stdafx.h"
#include "element.h"

template<class K, class V, int M = 3>
struct BTreeNode
{
    K _keys[M];
    V _values[M];
    bool _deleted[M];

    BTreeNode<K, V, M>* _subs[M + 1];
    size_t _size;

    BTreeNode<K, V, M>* _parent;

    BTreeNode()
        :_size(0) , _parent(nullptr)
    {
        for (size_t i = 0; i < M + 1; ++i)
        {
            _subs[i] = nullptr;
        }
        for (size_t i = 0; i < M ; ++i)
        {
            _deleted[i] = false;
        }
    }
};

template<class K, class V>
struct Pair
{
    K _first;
    V _second;

    Pair(const K& k = K(), const V& v = V())
        :_first(k), _second(v)
    {}
};

template<class K, class V, int M = 3>
class BTree
{
    typedef BTreeNode<K, V, M> Node;
public:
    BTree()
        :_root(nullptr)
    {}

    Pair<Node*, int> Find(const K& key)
    {
        Node* parent = nullptr;
        Node* cur = _root;
        while (cur)
        {
            size_t i = 0;
            while (i < cur->_size && cur->_keys[i] < key)
            {
                ++i;
            }

            if (cur->_keys[i] == key)
            {
                return Pair<Node*, int>(cur, i);
            }

            parent = cur;
            cur = cur->_subs[i];
        }

        return Pair<Node*, int>(parent, -1);
    }

    V operator[] (const K& k)
    {
        auto searchResult = Find(k);
        if (searchResult._second > 0 && !( searchResult._first -> _deleted[searchResult._second]) )
        {
            return searchResult._first -> _values[searchResult._second];
        }
        return searchResult._second;
    }

    bool Insert(const K& key, const V& value)
    {
        if (_root == nullptr)
        {
            _root = new Node;
            _root->_keys[0] = key;
            _root->_values[0] = value;
            ++_root->_size;

            return true;
        }

        Pair<Node*, int> ret = Find(key);
        if (ret._second != -1)
        {
            return false;
        }

        K k = key;
        V v = value;
        Node* cur = ret._first;
        Node* sub = nullptr;

        while (1)
        {
            _InsertKey(cur, k, v, sub);
            if (cur->_size < M)
            {
                return true;
            }

            // split
            int boundary = M / 2;

            Node* tmp = new Node;
            size_t index = 0;
            size_t size = cur->_size;

            for (size_t i = boundary + 1; i < size; ++i)
            {
                tmp->_values[index] = cur->_values[i];
                tmp->_deleted[index] = cur->_deleted[i];
                tmp->_keys[index++] = cur->_keys[i];
                tmp->_size++;
                cur->_size--;
            }
            // copy children
            index = 0;
            for (size_t i = boundary + 1; i <= size; ++i)
            {
                tmp->_subs[index] = cur->_subs[i];
                if (tmp->_subs[index])
                    tmp->_subs[index]->_parent = tmp;

                ++index;
            }

            k = cur->_keys[boundary];
            v = cur->_values[boundary];
            cur->_size--;

            // no parent
            if (cur->_parent == nullptr)
            {
                _root = new Node;
                _root->_keys[0] = k;
                _root->_values[0] = v;
                _root->_subs[0] = cur;
                _root->_subs[1] = tmp;
                _root->_size = 1;

                tmp->_parent = _root;
                cur->_parent = _root;

                return true;
            }

            cur = cur->_parent;
            sub = tmp;
        }
    }

    void _InsertKey(Node* cur, const K& k, const V& v, Node* sub)
    {
        int i = cur->_size - 1;
        while (i >= 0)
        {
            if (cur->_keys[i] > k)
            {
                cur->_keys[i + 1] = cur->_keys[i];
                cur->_values[i + 1] = cur->_values[i];
                cur->_subs[i + 2] = cur->_subs[i + 1];

                --i;
            }
            else
            {
                break;
            }
        }

        cur->_keys[i + 1] = k;
        cur->_values[i + 1] = v;
        cur->_subs[i + 2] = sub;
        if (sub)
        {
            sub->_parent = cur;
        }

        cur->_size++;
    }

    void InOrder()
    {
        _InOrder(_root);
        cout << endl;
    }

    void _InOrder(Node* root)
    {
        if (root == nullptr)
        {
            return;
        }

        for (size_t i = 0; i < root->_size; ++i)
        {
            _InOrder(root->_subs[i]);
            cout << root->_keys[i] << " ";
        }

        _InOrder(root->_subs[root->_size]);
    }

    void Delete(const K& k)
    {
        auto searchResult = Find(k);
        if (searchResult._second > 0)
        {
            searchResult._first -> _deleted[searchResult._second] = true;
        }
    }
protected:
    Node* _root;
};



// template <class keyT, class valueT>
// class TreeNode
// {
// public:
//     TreeNode<keyT, valueT> * parent;
//     vector<keyT> key;
//     vector<valueT> value;
//     vector<TreeNode<keyT, valueT>*> child;
//
//     TreeNode() :
//         parent(nullptr), child(vector<TreeNode<keyT, valueT>*> (nullptr)){};
//     // TreeNode(keyT k, ) :
//
// };
//
// template <class keyT, class valueT>
// class BPlusTree
// {
// public:
//     int size;
//     int order;
//     TreeNode<keyT, valueT> * root;
//     TreeNode<keyT, valueT> * activeNode;
//     void solveOverflow(TreeNode<keyT, valueT> *);
//     void solveUnderflow(TreeNode<keyT, valueT> *);
//
//     valueT search(keyT k);
//     bool insert(keyT k, valueT v);
//     bool remove(keyT k);
//     BPlusTree(int _order) : size(0), order(_order), root(nullptr), activeNode(nullptr){};
// };
//
//
// template <class keyT, class valueT>
// valueT BPlusTree<keyT, valueT>::search(keyT k)
// {
//     TreeNode<keyT, valueT> * thisNode = root;
//     activeNode = root;
//
//     while(activeNode)
//     {
//
//     }
// }


#endif
