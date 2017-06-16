#ifndef RecordBlock_hpp
#define RecordBlock_hpp

#include<string>
using std::string;

#define RECORD_LENGTH 4096

class RecordBlock {
public:
	string tablename;	
	char records[RECORD_LENGTH];	

	// blockNumber;
	RecordBlock* prev, *next;

	bool dirty;		
	bool locked;	

	RecordBlock();
	//~RecordBlock();
};

#endif//RecordBlock_hpp