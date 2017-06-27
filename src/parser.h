#ifndef _PARSER_H_
#define _PARSER_H_
#include "stdafx.h"
#include "API.h"
// #include

class Parser
{
private:
	API dbAPI;
	void parseSQL(string sqlStatement);

	void parseCreateTable(string statement);
	void parseDropTable(string statement);

	void parseCreateIndex(string statement);
	void parseDropIndex(string statement);

	void parseSelect(string statement);
	void parseInsert(string statement);
	void parseDelete(string statement);

public:
	Parser(){};

	void parseFile(string filename);
	void commandOperation();

};

#endif
