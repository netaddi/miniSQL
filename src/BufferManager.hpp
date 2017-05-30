#ifndef BufferManager_hpp
#define BufferManager_hpp

#include<map>

#include"IndexBlock.hpp"
#include"RecordBlock.hpp"

#define Record_Block_Capacity  10000
#define Index_Block_Capacity 10000

class BufferManager {
private:
	std::map<std::string, RecordBlock*>RecordBlockMap;
	std::map<std::string, IndexBlock*>IndexBlockMap;

	RecordBlock *head, *tail,*entries;
	IndexBlock *head, *tail,*entries;

	RecordBlock* newRecordBlock();
	IndexBlock* newIndexBlock();

	void remove(RecordBlock* block);
	void remove(IndexBlock* block);

private:
	void attach(RecordBlock* block);
	void attach(IndexBlock* block);

	void detach(RecordBlock* block);
	void detach(IndexBlock* block);
public:
	BufferManager(int size);
	~BufferManager();

	bool createTable(std::string tableName);
	bool dropTable(std::string tableName);
	RecordBlock* getRecordBlock(std::string tableName);

	IndexBlock* getIndexBlock(std::string tableName);

};

#endif//BufferManager_hpp