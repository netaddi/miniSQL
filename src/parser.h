#ifndef _PARSER_H_
#define _PARSER_H_
#include "stdafx.h"
// #include

class Parser
{
private:
	void parseSQL(string sqlStatement);

	void parseCreateTable(istringstream inputSQLStream);
	void parseDropTable(istringstream inputSQLStream);

	void parseCreateIndex(istringstream inputSQLStream);
	void parseDropIndex(istringstream inputSQLStream);

	void parseSelect(istringstream inputSQLStream);
	void parseInsert(istringstream inputSQLStream);
	void parseDelete(istringstream inputSQLStream);

public:
	Parser(){};

	void parseFile(string filename);
	void commandOperation();

};

#endif
