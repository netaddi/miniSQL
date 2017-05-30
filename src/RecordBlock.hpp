#ifndef RecordBlock_hpp
#define RecordBlock_hpp

#include<string>
using std::string;

#define RECORD_LENGTH 512	//每条记录长度
#define EACH_BLOCK_RECORDS 16	//每个块的最大记录数

class Record {
public:
	bool empty;
	char data[RECORD_LENGTH];
};

class RecordBlock {
public:
	std::string tablename;	//表名称
	Record records[EACH_BLOCK_RECORDS];	//记录
	int recordnum;	//现有的记录数目

	RecordBlock *pre, *next;	//指向前一块和后一块的指针

	bool dirty;		//是否被修改过
	bool locked;	//是否被锁定

	RecordBlock();
	~RecordBlock();
};

#endif//RecordBlock_hpp