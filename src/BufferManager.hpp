#ifndef BufferManager_hpp
#define BufferManager_hpp

#include<map>
using std::map;
//#include<list>
//using std::list;

#include"IndexBlock.hpp"
#include"RecordBlock.hpp"

#define Record_Block_Capacity  20000
#define Index_Block_Capacity 20000

//BufferManager--双链表实现
class BufferManager {
private:
	map<string, RecordBlockPointer>RecordBlockMap;
	map<string, IndexBlockPointer>IndexBlockMap;

	//list<RecordBlock>RecordPool;
	//list<IndexBlock>IndexPool;
	IndexBlockPointer ihead, itail;
	RecordBlockPointer rhead, rtail;

	RecordBlockPointer getNewPoolBlock();
	IndexBlockPointer getNewPoolBlock();

	void closeBlock(RecordBlockPointer pos);
	void closeBlock(IndexBlockPointer pos);

	bool maxSizeRecord();
	bool maxSizeIndex();

	int RecordSize;
	int IndexSize;

public:
	BufferManager();
	BufferManager(int size);
	~BufferManager();

	RecordBlockPointer getRecordBlock(string tableName);
	bool craeteTable(string tableName);
	bool dropTable(string tableName);
	
	IndexBlockPointer getIndexBlock(string tableName,string attr,string indexName);
	IndexBlockPointer newIndexBlock(string tableName, string attr, string indexName);
	bool deleteIndexBlock(string tableName, string attr, string indexName);
};

/*
//BufferManager--数组的实现
class BufferManager {
private:
	map<string, RecordBlockPointer>RecordBlockMap;
	map<string, IndexBlockPointer>IndexBlockMap;

	RecordBlock* RecordPool[Record_Block_Capacity];
	IndexBlock* IndexPool[Index_Block_Capacity];

	int curRecordPos;
	int curIndexPos;

	RecordBlockPointer getNewPoolBlock();
	IndexBlockPointer getNewPoolBlock();

	void closeBlock(RecordBlockPointer pos);
	void closeBlock(IndexBlockPointer pos);

	bool maxSizeRecord();
	bool maxSizeIndex();

public:
	BufferManager();
	BufferManager(int size);
	~BufferManager();

	RecordBlockPointer getRecordBlock(string tableName);
	bool craeteTable(string tableName);
	bool dropTable(string tableName);

	IndexBlockPointer getIndexBlock(string tableName, string attr, string indexName);
	IndexBlockPointer newIndexBlock(string tableName, string attr, string indexName);
	bool deleteIndexBlock(string tableName, string attr, string indexName);
};
*/

#endif//BufferManager_hpp