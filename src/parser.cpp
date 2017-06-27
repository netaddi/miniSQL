#include "parser.h"
#include "element.h"
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
            // sqlCommand += " ;";
            cout << "\n--Excecuting SQL statement: \n" << sqlCommand << ";\n";
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
        // cout << "----tokenized sql: " << sqlStatement << endl;

        istringstream inputSQLStream(sqlStatement);
        string token0, token1;
        inputSQLStream >> token0 >> token1;
        if (token0 == "create" && token1 == "table")
        {
            parseCreateTable(sqlStatement);
            return;
        }
        if (token0 == "drop" && token1 == "table")
        {
            parseDropTable(sqlStatement);
            return;
        }
        if (token0 == "create" && token1 == "index")
        {
            parseCreateIndex(sqlStatement);
            return;
        }
        if (token0 == "drop" && token1 == "index")
        {
            parseDropIndex(sqlStatement);
            return;
        }
        if (token0 == "select")
        {
            parseSelect(sqlStatement);
            return;
        }
        if (token0 == "insert" && token1 == "into")
        {
            parseInsert(sqlStatement);
            return;
        }
        if (token0 == "delete" && token1 == "from")
        {
            parseDelete(sqlStatement);
            return;
        }
        cout << "Error at parsing SQL statement : No operation: " << token0 << ". Aborted." <<  endl;
        return;
    }
    catch(exception & e)
    {
        cout << "Error at tokenizing SQL statement. Execution Aborted." << endl;
        return;
    }
}


void Parser::parseCreateTable(string statement)
{
    istringstream inputSQLStream(statement);
    string currentToken;

    // ignore first two tokens.
    inputSQLStream >> currentToken >> currentToken >> currentToken;
    string tableName = currentToken;
    string primaryKey = "";

    inputSQLStream >> currentToken;
    if (currentToken != "(")
    {
        cout << "Error : Expected '(' after the table name. Aborted." << endl;
        return;
    }

    // parse attributes
    vector<Attribute> attributes;
    while (true)
    {
        if (currentToken == ")")
        {
            break;
        }
        // read info of each attribute
        string attrToken, typeToken;
        inputSQLStream >> attrToken >> typeToken;

        // case int
        if (typeToken == "int")
        {
            inputSQLStream >> currentToken;
            if (currentToken == "unique")
            {
                attributes.push_back(Attribute(attrToken, INT, sizeof(int), true));
                inputSQLStream >> currentToken;
            }
            else
            {
                attributes.push_back(Attribute(attrToken, INT, sizeof(int), false));
            }
            continue;
        }
        if (typeToken == "float")
        {
            inputSQLStream >> currentToken;
            if (currentToken == "unique")
            {
                attributes.push_back(Attribute(attrToken, FLOAT, sizeof(float), true));
                inputSQLStream >> currentToken;
            }
            else
            {
                attributes.push_back(Attribute(attrToken, FLOAT, sizeof(float), false));
            }
            continue;
        }
        if (typeToken == "char" || typeToken == "varchar" || typeToken == "string")
        {
            int size;
            inputSQLStream >> currentToken >> size >> currentToken >> currentToken;
            if (currentToken == "unique")
            {
                attributes.push_back(Attribute(attrToken, STRING, size, true));
                inputSQLStream >> currentToken;
            }
            else
            {
                attributes.push_back(Attribute(attrToken, STRING, size, false));
            }
            continue;
        }
        if(attrToken == "primary" && typeToken == "key")
        {
            inputSQLStream >> currentToken >> primaryKey >> currentToken >> currentToken;
            continue;
        }
    }
    TableInfo newTable(tableName, primaryKey, attributes);
    dbAPI.createTable(newTable);
}

void Parser::parseDropTable(string statement)
{

}


void Parser::parseCreateIndex(string statement)
{

}

void Parser::parseDropIndex(string statement)
{

}


void Parser::parseSelect(string statement)
{

}

void Parser::parseInsert(string statement)
{

}

void Parser::parseDelete(string statement)
{

}
