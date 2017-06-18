#ifndef IndexBlock_hpp
#define IndexBlock_hpp

#include<string>
using std::string;
#define INDEX_LENGTH 4096

class IndexBlock {
public:
	string tableName;
	string attributeName;
	string indexName;
	char address[INDEX_LENGTH];

	bool dirty;
	bool locked;

	//int blockNumber;
	IndexBlock* prev, *next;

	IndexBlock();
	//~IndexBlock();
};

#endif//IndexBlock_hpp