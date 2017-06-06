#ifndef RecordManager_hpp
#define RecordManager_hpp

#include "element.h"
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
	typedef map<string, vector<Element>> Instruction;
public:
	RecordManager();
	~RecordManager();

	bool createTable(string tableName);
	bool dropTable(string tableName);

	bool insertRecord(string tableName,Record &record);	
	bool deleteRecord(string tableName,vector<Element>&input);

	bool optimize(string tableName, vector<Element> &input, Instruction &instruction);

	bool selectRecord(string tableName, vector<Element> &input);

	void deleteAllIndex(const Record & record, string tableName);

	void printRecord(const Record &record);

};

#endif//RecordManager_hpp