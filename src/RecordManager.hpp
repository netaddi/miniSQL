#ifndef RecordManager_hpp
#define RecordManager_hpp

#include "CommonHeader.hpp"
#include "RecordBlock.hpp"
#include "CatalogManager.hpp"
#include "BufferManager.hpp"

#include <iostream>
#include <fstream>
#include<map>
using std::map;
#include<vector>
using std::vector;

class RecordManager {
private:
	typedef map<string, vector<Value>> Instruction;
public:
	RecordManager();
	~RecordManager();

	bool createTable(string tableName);
	bool dropTable(string tableName);
	bool insertRecord(string tableName,Record &record);
	bool optimize(string tableName, vector<Value> &input, Instruction &instruction);
	bool deleteRecord(string tableName,vector<Value>&input);
	bool selectRecord(string tableName, vector<Value> &input);
	void deleteIndex(const Record & record, string tableName);
	void printRecord(const Record &record);

};

#endif//RecordManager_hpp