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
	int blockId;
	char address[INDEX_LENGTH];

	bool dirty;
	bool locked;
	bool active;
	//int blockNumber;
	IndexBlock* prev, *next;

	IndexBlock():
		tableName(""), attributeName(""), indexName(""),
		dirty(false), locked(false), active(false),
		prev(nullptr), next(nullptr){};

	IndexBlock(string table, string attr, int blockId): tableName(table), attributeName(attr), blockId(blockId),
		dirty(false), locked(false), active(false),
		prev(nullptr), next(nullptr){};

	// IndexBlock(const IndexBlock& ib):{};
	//~IndexBlock();
};

#endif//IndexBlock_hpp
