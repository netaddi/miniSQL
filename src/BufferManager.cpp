#include "BufferManager.h"
#include "stdafx.h"
#if DBG
// #include "debug.h"
#endif

// =============== BufferPage ==============

inline string BufferPage::getPageFileName()
{
	return "data/record/_TBL_" + table
	     + "__PageID_" + to_string(pageId) + "___";
}

inline bool BufferPage::isFull()
{
	return (usedLength + recordLength >= PAGE_SIZE);
}

bool BufferPage::writeBackPage()
{
	#if DBG
	cout << "filename : " << pageFileName << endl;
	cout << "length : " << usedLength << endl;
	#endif
	if (dirty)
	{
		ofstream pageWriteStream;
		pageWriteStream.open(pageFileName, ios::out | ios::binary);
		// write the used page first.
		pageWriteStream.write(reinterpret_cast<const char *> (&usedLength), sizeof(int));
		pageWriteStream.write(data, PAGE_SIZE);
		pageWriteStream.close();
		dirty = pageWriteStream.bad();
		return pageWriteStream.good();
	}
	// else the page is not changed
	return true;
}

bool BufferPage::reloadPage()
{
	dirty = false;
	ifstream pageReadStream;
	pageReadStream.open(pageFileName, ios::in | ios::binary);
	pageReadStream.read(reinterpret_cast<char * > (&usedLength), sizeof(int));
	pageReadStream.read(data, PAGE_SIZE);
	pageReadStream.close();
	return pageReadStream.good();
}

int BufferPage::insertDataToPage(char * newData)
{
	if (isFull())
	{
		return -1;
	}
	else
	{
		for (int i = 0; i < recordLength; i++)
		{
			data[usedLength + i] = newData[i];
		}
		// #if DBG
		// 	cout << "read lenght: " << recordLength << endl;
		// 	printData(data + usedLength, recordLength);
		// #endif
		usedLength += recordLength;
		dirty = true;
		return (usedLength - recordLength);
	}
}

char * BufferPage::readRecordWithOffset(int offset)
{
	// char * returnData[recordLength];
	// for (int i = 0; i < recordLength; i++)
	// {
	//
	// }

	// #if DBG
	// 	cout << "read lenght: " << recordLength << endl;
	// 	printData(data + offset, recordLength);
	// #endif
	return (data + offset);
}

bool BufferPage::deleteWithOffset(int offset)
{
	if (recordLength + offset > PAGE_SIZE)
	{
		return false;
	}
	else
	{
		for (int i = 0; i < recordLength; i++)
		{
			data[offset + i] = 255;
		}
	}
	return true;
}

bool BufferPage::deleteWithCheckFunc(function<bool (char * )> checkDeletionFunction)
{
	for (int offset = 0; offset < usedLength; offset += recordLength)
	{
		if (checkDeletionFunction(data + offset))
		{
			deleteWithOffset(offset);
		}
	}
	return true;
}


// =============== BufferPool ====================

BufferPool::BufferPool(TableInfo & table):
	table(table.tableName), recordLength(table.recordLength)
{
	activePageId = 0;
	validPageLength = PAGE_SIZE - (PAGE_SIZE % recordLength);
	reloadBuffer();
	cout << "size: " << buffers.size() << endl;
	if (buffers.size() == 0)
	{
		#if DBG
			cout << "no buffer read from file" << endl;
		#endif
		addNewBufferToPool();
	}
	findactivePageId();
	#if DBG
		cout << "activePageId: " <<  activePageId << endl;
	#endif
};

inline string BufferPool::getPageFileName(int id)
{
	return "data/record/_TBL_" + table
	     + "__PageID_" + to_string(id) + "___";
}


void BufferPool::addNewBufferToPool()
{
	#if DBG
		cout << "append buffer page" << endl;
	#endif
	buffers.push_back(new BufferPage(table, recordLength, buffers.size()));
	pageFull.push_back(false);
	// bufferCount++;
	dirty = true;
};

bool BufferPool::reloadBuffer()
{
	buffers.clear();
	pageFull.clear();
	int currentId = 0;
	while(checkFileExistance(getPageFileName(currentId)))
	{
		BufferPage* thisPage = new BufferPage(table, recordLength, currentId);
		if(thisPage -> reloadPage())
		{
			buffers.push_back(thisPage);
			pageFull.push_back(thisPage -> isFull());
			if (activePageId < 0 && (!(thisPage -> isFull())))
			{
				activePageId = currentId;
			}
			currentId++;
		}
		else
		{
			return false;
		}
	}
	dirty = false;
	return true;
}

bool BufferPool::writeBackBuffers()
{
	if(dirty)
	{
		bool noError = all_of(buffers.begin(), buffers.end(),
							[](auto page)
							{
								return page -> writeBackPage();
							});
		dirty = !noError;
		return noError;
	}
	return true;
}

void BufferPool::findactivePageId()
{
	if (pageFull[activePageId])
	{
		for (int i = 0; i < (int)pageFull.size(); i++)
		{

			if (!pageFull[i])
			{
				activePageId = i;
				return;
			}
		}
		#if DBG
			cout << "no free buffer!" << endl;
		#endif
		addNewBufferToPool();
		activePageId = buffers.size() - 1;
		return;
	}
	// else
	return;
}

int BufferPool::insertData(char * newData)
{
	int pageOffset = buffers[activePageId] -> insertDataToPage(newData);
	int totalOffset = activePageId * PAGE_SIZE + pageOffset;
	pageFull[activePageId] = buffers[activePageId] -> isFull();
	findactivePageId();
	dirty = true;
	return totalOffset;
}

char * BufferPool::queryWithOffset(int offset)
{
	int id = offset / PAGE_SIZE;
	int pageOffset = offset % PAGE_SIZE;
	#if DBG
		cout << id << " " << pageOffset << endl;
	#endif
	return buffers[id] -> readRecordWithOffset(pageOffset);
}

int BufferPool::getTotalBufferSize()
{
	return accumulate(buffers.begin(), buffers.end(), 0,
						[](int sum, auto page)
						{
							return sum + page -> usedLength;
						});
}

char * BufferPool::queryAllData()
{
	// remember to delete !!!!!!
	#if DBG
		cout << "all size: " << getTotalBufferSize() << endl;
	#endif
	char * returnData = new char[getTotalBufferSize()];
	int dataPtr = 0;
	int pagePtr;
	for (auto const & page : buffers)
	{
		pagePtr = 0;
		while(pagePtr < page -> usedLength)
		{
			returnData[dataPtr++] = page -> data[pagePtr++];
		}
	}
	// printData(returnData, 40);
	return returnData;
}

bool BufferPool::deleteWithOffset(int offset)
{
	int id = offset / PAGE_SIZE;
	int pageOffset = offset % PAGE_SIZE;
	if (id < (int)buffers.size())
	{
		return buffers[id] -> deleteWithOffset(pageOffset);
	}
	return false;
}

bool BufferPool::deleteWithCheckFunc(function<bool (char * )> checkDeletionFunction)
{
	for_each(buffers.begin(), buffers.end(), [=](auto page)
												{
													page -> deleteWithCheckFunc(checkDeletionFunction);
												});
	return true;
}

// ================= BufferManager ======================

void BufferManager::createBufferForTable(TableInfo & table)
{
	if (bufferMap.find(table.tableName) == bufferMap.end())
	{
		auto newPool = new BufferPool(table);
		// newPool -> addNewBufferToPool();
		// newPool -> reloadBuffer();
		bufferMap[table.tableName] = newPool;
	}
}

bool BufferManager::writeBackTable(string table)
{
	if (bufferMap.find(table) != bufferMap.end())
	{
		return bufferMap[table] -> writeBackBuffers();
	}
	return false;
}

bool BufferManager::reloadTable(string table)
{
	if (bufferMap.find(table) != bufferMap.end())
	{
		return bufferMap[table] -> reloadBuffer();
	}
	return false;
}

bool BufferManager::writeBackAll()
{
	return all_of(bufferMap.begin(), bufferMap.end(),
					[](auto bufferMapIter)
					{
						// cout << bufferMapIter.first;
						return bufferMapIter.second -> writeBackBuffers();
					});
}

int BufferManager::insertIntoTable(string table, char * newData)
{
	if (bufferMap.find(table) != bufferMap.end())
	{
		return bufferMap[table] -> insertData(newData);
	}
	#if DBG
		cout << "not found!" << endl;
	#endif
	return -1;
}

char * BufferManager::queryTableWithOffset(string table, int offset)
{
	if (bufferMap.find(table) != bufferMap.end())
	{
		return bufferMap[table] -> queryWithOffset(offset);
	}
	return nullptr;
}

char * BufferManager::queryCompleteTable(string table)
{
	if (bufferMap.find(table) != bufferMap.end())
	{
		return bufferMap[table] -> queryAllData();
	}
	return nullptr;
}

int BufferManager::getTableBufferSize(string table)
{
	if (bufferMap.find(table) != bufferMap.end())
	{
		return bufferMap[table] -> getTotalBufferSize();
	}
	return 0;
}

bool BufferManager::deleteFromTableWithOffset(string table, int offset)
{
	if (bufferMap.find(table) != bufferMap.end())
	{
		return bufferMap[table] -> deleteWithOffset(offset);
	}
	return false;
}

bool BufferManager::deleteFromTableWithCheckFunc(string table, function<bool (char *)> checkDeletionFunction)
{
	if (bufferMap.find(table) != bufferMap.end())
	{
		return bufferMap[table] -> deleteWithCheckFunc(checkDeletionFunction);
	}
	return false;
}
