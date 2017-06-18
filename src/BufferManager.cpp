#include"BufferManager.hpp"

#include<fstream>
#include<assert.h>
using namespace std;

//private functions

//lock the block so it cannot be replaced
void BufferManager::lockBlock(RecordBlock* pos) {
	pos->locked = true;
}
void BufferManager::lockBlock(IndexBlock* pos) {
	pos->locked = true;
}

//delete a block from list
void BufferManager::deattach(RecordBlock* pos) {
	assert(pos);
	if (pos == rhead)
		rhead = pos->next;
	else
		pos->prev->next = pos->next;
	if (pos == rtail)
		rtail = pos->prev;
	else
		pos->next->prev = pos->prev;
}
void BufferManager::deattach(IndexBlock* pos) {
	assert(pos);
	if (pos == ihead)
		ihead = pos->next;
	else
		pos->prev->next = pos->next;
	if (pos == itail)
		itail = pos->prev;
	else
		pos->next->prev = pos->prev;
}

//insert a new block to list head
void BufferManager::attach(RecordBlock* pos) {
	if (pos == nullptr)
		return;
	if (rhead == nullptr)
	{
		rhead = pos;
		rhead->prev = rhead->next = nullptr;
	}
	else
	{
		rhead->prev = pos;
		pos->next = rhead;
		rhead = pos;
	}
}
void BufferManager::attach(IndexBlock* pos) {
	if (pos == nullptr)
		return;
	if (ihead == nullptr)
	{
		ihead = pos;
		ihead->prev = ihead->next = nullptr;
	}
	else
	{
		ihead->prev = pos;
		pos->next = ihead;
		ihead = pos;
	}
}

//public functions
BufferManager::BufferManager() {
	ihead = itail = nullptr;
	rhead = rtail = nullptr;
	RecordSize = IndexSize = 0;
	
}

/*BufferManager::BufferManager(int size) {
	ihead = itail = nullptr;
	rhead = rtail = nullptr;
	for (int i = 0; i < size; i++)
	{
		RecordBlock* emptyRecord = new RecordBlock();
	}
	for (int i = 0; i < size; i++)
	{
		IndexBlock* emptyIndex = new IndexBlock();
	}
	RecordSize = IndexSize = size;
}*/

BufferManager::~BufferManager() {
	RecordBlock* cur1,*tmp1;
	IndexBlock* cur2,*tmp2;
	for (cur1 = rtail; cur1 !=nullptr ;cur1=cur1->prev)
	{
		tmp1 = cur1->prev;
		closeBlock(cur1);
		cur1 = tmp1;
	}
	for (cur2 = itail; cur2 != nullptr; cur2 = cur2->prev)
	{
		tmp2 = cur2->prev;
		closeBlock(cur1);
		cur2 = tmp2;
	}
}

//get empty new record block
RecordBlock* BufferManager::getNewPoolRecordBlock() {
	RecordBlock* newhead = new RecordBlock;
	newhead->next = rhead;
	rhead->prev = newhead;
	newhead->prev = nullptr;
	rhead = newhead;
	RecordSize++;
	return rhead;
}
//get empty new index block
IndexBlock* BufferManager::getNewPoolIndexBlock() {
	IndexBlock* newhead = new IndexBlock;
	newhead->next = ihead;
	ihead->prev = newhead;
	newhead->prev = nullptr;
	ihead = newhead;
	IndexSize++;
	return ihead;
}

//write to files and remove this block and map
void BufferManager::closeBlock(RecordBlock* pos) {
	ofstream fout;
	if (pos->locked) {
		return;
	}
	string path = "data/record" + pos->tablename;
	string fileName = pos->tablename + "";	//need to modify
	fout.open(path + fileName, ios::out| ios::binary);
	if (fout)
	{
		fout.write((char*)pos->records,sizeof(pos->records));
		fout.close();
	}
	deattach(pos);
	delete(pos);
	RecordSize--;

	unordered_map<string,RecordBlock*>::iterator it = RecordBlockMap.find(fileName);
	RecordBlockMap.erase(it);
}

void BufferManager::closeBlock(IndexBlock* pos) {
	ofstream fout;
	if (pos->locked) {
		return;
	}
	string path = "data/index" + pos->tableName+pos->attributeName;
	string fileName = pos->tableName + pos->attributeName +pos->indexName;//need to modify
	fout.open(path + fileName, ios::out| ios::binary);
	if (fout)
	{
		fout.write(pos->address,4096);
		fout.close();
	}
	deattach(pos);
	delete(pos);
	IndexSize--;
	unordered_map<string, IndexBlock*>::iterator it = IndexBlockMap.find(fileName);
	IndexBlockMap.erase(it);
}

//search this block;if not exist,read from file
RecordBlock* BufferManager::getRecordBlock(string tableName) {
	string path = "data/record" + tableName;
	string fileName = tableName + "";	//need to modify
	unordered_map<string, RecordBlock*>::iterator it = RecordBlockMap.find(fileName);
	if (it != RecordBlockMap.end())
	{
		deattach(it->second);
		attach(it->second);
	}
	else
	{
		ifstream fin;
		fin.open(path + fileName, ios::in| ios::binary);
		if (fin)
		{
			RecordBlock* newhead = getNewPoolRecordBlock();
			fin.read(newhead->records, sizeof(newhead->records));
			newhead->tablename = tableName;
			RecordBlockMap[fileName] = newhead;
			fin.close();
		}
	}
	return rhead;
}

bool BufferManager::createTable(string tableName) {
	string path = "data/record" ;
	string fileName = tableName;
	ofstream fout;
	fout.open(path+fileName,ios::out |ios::binary);
	if (fout)
	{
		fout.close();
		return true;
	}
	else
		return false;
}

bool BufferManager::dropTable(string tableName) {
	string path = "data/record";
	string fileName = tableName + "";	//
	unordered_map<string,RecordBlock*>::iterator it = RecordBlockMap.find(fileName);
	if (it != RecordBlockMap.end())
	{
		deattach(it->second);
		delete(it->second);
		RecordSize--;
		RecordBlockMap.erase(it);
		string tmp = path + fileName;
		return remove(tmp.c_str());
	}
	else
		return false;
}

//search this block;if not exist,read from file;if not exist such file,create it
IndexBlock* BufferManager::getIndexBlock(string tableName, string attr, string indexName) {
	string path = "data/index" + tableName + attr;
	string fileName = tableName + attr + indexName;//need to modify
	unordered_map<string, IndexBlock*>::iterator it = IndexBlockMap.find(fileName);
	if (it != IndexBlockMap.end())
	{
		deattach(it->second);
		attach(it->second);
		return ihead;
	}
	else
	{
		ifstream fin;
		fin.open(path + fileName, ios::in| ios::binary);
		if (fin)
		{
			IndexBlock* newhead = getNewPoolIndexBlock();
			fin.read(newhead->address, sizeof(newhead->address));
			newhead->tableName = tableName;
			newhead->attributeName = attr;
			newhead->indexName = indexName;
			IndexBlockMap[fileName] = newhead;
			return ihead;
		}
		else
		{
			createIndex(tableName,attr,indexName);
			return getIndexBlock(tableName, attr, indexName);
		}
	}
}

bool BufferManager::createIndex(string tableName, string attr, string indexName) {
	string path = "data/index" + tableName + attr;
	string fileName = tableName + attr + indexName;
	ofstream fout;
	fout.open(path + fileName, ios::out | ios::binary);
	if (fout)
	{
		fout.close();
		return true;
	}
	else
		return false;
}

bool BufferManager::deleteIndexBlock(string tableName, string attr, string indexName) {
	string path = "data/index" + tableName + attr;
	string fileName = tableName + attr + indexName;//need to modify
	unordered_map<string, IndexBlock*>::iterator it = IndexBlockMap.find(fileName);
	if (it != IndexBlockMap.end())
	{
		deattach(it->second);
		delete(it->second);
		IndexSize--;
		IndexBlockMap.erase(it);
		string tmp = path + fileName;
		return remove(tmp.c_str());
	}
	else
		return false;
}