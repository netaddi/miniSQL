#ifndef IndexBlock_hpp
#define IndexBlock_hpp

#include<string>
using std::string;

typedef IndexBlock* IndexBlockPointer;

class IndexBlock {
public:
	string tableName;
	string attributeName;
	string indexName;

	bool dirty;
	bool locked;

	//int blockNumber;
	IndexBlockPointer prev, next;

	IndexBlock();
	~IndexBlock();
};

#endif//IndexBlock_hpp