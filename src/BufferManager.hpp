#ifndef BufferManager_hpp
#define BufferManager_hpp

//#include<map>
#include<unordered_map>
using namespace std;

#include"IndexBlock.hpp"
#include"RecordBlock.hpp"

#define Record_Block_Capacity  20000
#define Index_Block_Capacity 20000

//BufferManager--using list and std::unordered_map
class BufferManager {
private:
	//map<string, RecordBlock*>RecordBlockMap;
	//map<string, IndexBlock*>IndexBlockMap;
	unordered_map<string, RecordBlock*>RecordBlockMap;
	unordered_map<string, IndexBlock*>indexBlockMap;

	IndexBlock* ihead, *itail;
	RecordBlock* rhead, *rtail;

	void lockBlock(RecordBlock* pos);
	void lockBlock(IndexBlock* pos);

	void deattach(RecordBlock* pos);
	void deattach(IndexBlock* pos);

	void attach(RecordBlock* pos);
	void attach(IndexBlock* pos);

	int RecordSize;
	int IndexSize;

public:
	BufferManager();
	//BufferManager(int size);
	~BufferManager();

	RecordBlock* getNewPoolRecordBlock();
	IndexBlock* getNewPoolIndexBlock();

	void closeBlock(RecordBlock* pos);
	void closeBlock(IndexBlock* pos);

	RecordBlock* getRecordBlock(string tableName);
	bool createTable(string tableName);
	bool dropTable(string tableName);

	IndexBlock* getIndexBlock(string tableName,string attr, int blockId);
	bool createIndex(string tableName, string attr, string indexName);
	bool deleteIndexBlock(string tableName, string attr, string indexName);
};

/*
//BufferManager--using array
class BufferManager {
private:
	map<string, RecordBlock*>RecordBlockMap;
	map<string, IndexBlock*>IndexBlockMap;

	RecordBlock* RecordPool[Record_Block_Capacity];
	IndexBlock* IndexPool[Index_Block_Capacity];

	int curRecordPos;
	int curIndexPos;

	RecordBlock* getNewPoolBlock();
	IndexBlock* getNewPoolBlock();

	void closeBlock(RecordBlock* pos);
	void closeBlock(IndexBlock* pos);

	bool maxSizeRecord();
	bool maxSizeIndex();

public:
	BufferManager();
	BufferManager(int size);
	~BufferManager();

	RecordBlock* getRecordBlock(string tableName);
	bool craeteTable(string tableName);
	bool dropTable(string tableName);

	IndexBlock* getIndexBlock(string tableName, string attr, string indexName);
	IndexBlock* newIndexBlock(string tableName, string attr, string indexName);
	bool deleteIndexBlock(string tableName, string attr, string indexName);
};
*/

#endif//BufferManager_hpp
