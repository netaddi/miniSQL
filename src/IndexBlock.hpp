#ifndef IndexBlock_hpp
#define IndexBlock_hpp

#include<string>
using std::string;

class IndexBlock {
public:
	std::string tablename;
	std::string attrname;
	char* address;

	bool dirty;
	bool locked;

	IndexBlock *pre, *next;

	IndexBlock();
	~IndexBlock();
};

#endif//IndexBlock_hpp