#include "parser.h"
// #include "element.h"
#include "stdafx.h"
#include "API.h"

void Parser::parseFile(string filename)
{
	ifstream sqlFileStream(filename);
	if (sqlFileStream)
	{
		string fileContent(
			(std::istreambuf_iterator<char>(sqlFileStream)),
			(std::istreambuf_iterator<char>()));
		vector<string> SQLcommandVector = split<string>(fileContent, ";");

		for(auto & sqlCommand : SQLcommandVector)
		{
            cout << "--Excecuting SQL statement: \n" << sqlCommand << ";\n";
			parseSQL(sqlCommand);
		}
	}
	else
	{
		cout << "could not open the file " << filename << '\n';
	}
};

void Parser::commandOperation()
{
	string sqlCommand;
	while(true)
	{
		string commandAppended = "";
		sqlCommand = "";
		cout << "sql>";
		while(true)
		{
			getline(cin, commandAppended);
			sqlCommand += commandAppended + " ";
			if (commandAppended[commandAppended.length() - 1] == ';')
			{
				break;
			}
			cout << "->";
		}
		if (sqlCommand == "exit;")
		{
			break;
		}
		parseSQL(sqlCommand);
	}
};

void Parser::parseSQL(string sqlStatement)
{
    try
    {
        sqlStatement = regex_replace(sqlStatement, regex("[\\(\\),]"), " $& ");
        sqlStatement = regex_replace(sqlStatement, regex("\\s+"), " ");
        // cout << "tokenized sql: " << sqlStatement << endl;

        istringstream inputSQLStream(sqlStatement);
        string token0, token1;
        inputSQLStream >> token0 >> token1;

    }
    catch(exception & e)
    {

    }


}
