#ifndef RecordBlock_hpp
#define RecordBlock_hpp

#include<string>
using std::string;

#define RECORD_LENGTH 512	
#define BLOCK_RECORDS 8	

typedef RecordBlock* RecordBlockPointer;

class Record {
public:
	char data[RECORD_LENGTH];
	bool empty;
};

class RecordBlock {
public:
	string tablename;	
	Record records[BLOCK_RECORDS];	
	int recordnum;	

	// blockNumber;
	RecordBlockPointer prev, next;

	bool dirty;		
	bool locked;	

	RecordBlock();
	~RecordBlock();
};

#endif//RecordBlock_hpp