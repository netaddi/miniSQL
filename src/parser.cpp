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
	        sqlCommand = regex_replace(sqlCommand, regex("^\\s+"), "");
			// #if !PRS_TEST_MODE
	            cout << "\n--Excecuting SQL statement: \n" << sqlCommand << ";\n";
				cout << "--result :\n";
			// #endif
			parseSQL(sqlCommand);
		}
		dbAPI.writeBackAll();
		cout << "finished executing file " << filename << endl;
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
			if (commandAppended.back() == ';')
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
		if (token0 == "source" || token0 == "exec")
		{
			parseFile(token1);
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
    istringstream inputSQLStream(statement);
    string tempToken, tableName;
    inputSQLStream >> tempToken >> tempToken >> tableName;
	dbAPI.deleteTable(tableName);
}


void Parser::parseCreateIndex(string statement)
{
    istringstream inputSQLStream(statement);
    string thisToken, indexName, tableName, AttributeName;
	//                create       index       <INDEX_NAME>  on           <TABLE>      (            <ATTRIBUTE>      )
    inputSQLStream >> thisToken >> thisToken >> indexName >> thisToken >> tableName >> thisToken >> AttributeName;
	IndexInfo newIndex(tableName, AttributeName, indexName);
	dbAPI.createIndex(newIndex);
}

void Parser::parseDropIndex(string statement)
{
    istringstream inputSQLStream(statement);
    string tempToken, indexName;
    inputSQLStream >> tempToken >> tempToken >> indexName;
	dbAPI.deleteIndex(indexName);
}

void Parser::parseInsert(string statement)
{
    istringstream inputSQLStream(statement);
    string thisToken, tableName;
	//                insert       into         <TABLE>      values       (
	inputSQLStream >> thisToken >> thisToken >> tableName >> thisToken >> thisToken;

	vector<Element *> elementsForRecord;
	string elementStartToken, elementEndToken, nextSignToken, finalElementToken;
	while(1)
	{
		inputSQLStream >> elementStartToken;
		if (elementStartToken[0] == '"')
		{
			elementEndToken = elementStartToken;
			while(elementEndToken.back() != '"')
			{
				inputSQLStream >> elementEndToken;
			}
			size_t startPos = statement.find(elementStartToken) + 1;
			size_t tokenLength = statement.find(elementEndToken) + elementEndToken.length() - 1 - startPos;
			finalElementToken = statement.substr(startPos, tokenLength);
		}
		else
		{
			finalElementToken = elementStartToken;
		}
		elementsForRecord.push_back(new Element(finalElementToken));
		inputSQLStream >> nextSignToken;
		if (nextSignToken == ")")
		{
			break;
		}
	}

	// for (auto& e : elementsForRecord)
	// {
	// 	cout << *e << endl;
	// }
	// cout << "====================\n";

	dbAPI.insertInto(tableName, elementsForRecord);

}

void Parser::parseSelect(string statement)
{
    istringstream inputSQLStream(statement);
    string tempToken, tableName, attributeToken, nextToken;
	vector<string> columns;
	inputSQLStream >> tempToken;
	do
	{
		inputSQLStream >> attributeToken >> nextToken;
		columns.push_back(attributeToken);
	} while(nextToken == ",");
	inputSQLStream >> tableName;
	string whereToken;
	inputSQLStream >> whereToken;
	vector<QueryBase *> queryVec;
	if (whereToken[0])
	{
		string compareAttribute, compratorToken, valueStartToken, valueEndToken, finalValueToken, nextToken;
		while(1)
		{
			inputSQLStream >> compareAttribute >> compratorToken >> valueStartToken;
			if (valueStartToken.front() == '"')
			{
				valueEndToken = valueStartToken;
				while(valueEndToken.back() != '"')
				{
					inputSQLStream >> valueEndToken;
				}
				size_t startPos = statement.find(valueStartToken) + 1;
				size_t tokenLength = statement.find(valueEndToken) + valueEndToken.length() - 1 - startPos;
				finalValueToken = statement.substr(startPos, tokenLength);
				finalValueToken.resize(dbAPI.getAttributeSize(tableName, compareAttribute));
			}
			else
			{
				finalValueToken = valueStartToken;
			}
			// to do : reinterpret value.
			QueryBase * thisQuery;
			if (compratorToken == "=" || compratorToken == "!=")
			{
				switch (dbAPI.getAttributeType(tableName, compareAttribute))
				{
					case INT:
						thisQuery = new SingleQuery<int>(compareAttribute, stoi(finalValueToken), compratorToken == "!=");
						break;
					case FLOAT:
						thisQuery = new SingleQuery<float>(compareAttribute, stof(finalValueToken), compratorToken == "!=");
						break;
					case STRING:
						thisQuery = new SingleQuery<string>(compareAttribute, finalValueToken, compratorToken == "!=");
						break;
				}
			}
			else
			{
				switch (dbAPI.getAttributeType(tableName, compareAttribute))
				{
					case INT:
						thisQuery = new infinityRangeQuery<int>(compareAttribute, stoi(finalValueToken), compratorToken);
						break;
					case FLOAT:
						thisQuery = new infinityRangeQuery<float>(compareAttribute, stof(finalValueToken), compratorToken);
						break;
					case STRING:
						thisQuery = new infinityRangeQuery<string>(compareAttribute, finalValueToken, compratorToken);
						break;
				}
			}
			queryVec.push_back(thisQuery);
			if (inputSQLStream.eof())
			{
				break;
			}
			else
			{
				inputSQLStream >> nextToken;
				if (nextToken != "and")
				{
					break;
				}
			}
		}

		dbAPI.selectFrom(tableName, columns, queryVec);
	}
	else
	{
		dbAPI.selectFrom(tableName, columns, {});
	}
}

void Parser::parseDelete(string statement)
{
    istringstream inputSQLStream(statement);
    string tempToken, tableName, attributeToken;
	inputSQLStream >> tempToken >> tempToken >> tableName >> tempToken;
	if (tempToken[0])
	{
		vector<QueryBase *> queryVec;
		string compareAttribute, compratorToken, valueStartToken, valueEndToken, finalValueToken, nextToken;
		while(1)
		{
			inputSQLStream >> compareAttribute >> compratorToken >> valueStartToken;
			if (valueStartToken.front() == '"')
			{
				valueEndToken = valueStartToken;
				while(valueEndToken.back() != '"')
				{
					inputSQLStream >> valueEndToken;
				}
				size_t startPos = statement.find(valueStartToken) + 1;
				size_t tokenLength = statement.find(valueEndToken) + valueEndToken.length() - 1 - startPos;
				finalValueToken = statement.substr(startPos, tokenLength);
				finalValueToken.resize(dbAPI.getAttributeSize(tableName, compareAttribute));
			}
			else
			{
				finalValueToken = valueStartToken;
			}
			// to do : reinterpret value.
			QueryBase * thisQuery;
			if (compratorToken == "=" || compratorToken == "!=")
			{
				switch (dbAPI.getAttributeType(tableName, compareAttribute))
				{
					case INT:
						thisQuery = new SingleQuery<int>(compareAttribute, stoi(finalValueToken), compratorToken == "!=");
						break;
					case FLOAT:
						thisQuery = new SingleQuery<float>(compareAttribute, stof(finalValueToken), compratorToken == "!=");
						break;
					case STRING:
						thisQuery = new SingleQuery<string>(compareAttribute, finalValueToken, compratorToken == "!=");
						break;
				}
			}
			else
			{
				switch (dbAPI.getAttributeType(tableName, compareAttribute))
				{
					case INT:
						thisQuery = new infinityRangeQuery<int>(compareAttribute, stoi(finalValueToken), compratorToken);
						break;
					case FLOAT:
						thisQuery = new infinityRangeQuery<float>(compareAttribute, stof(finalValueToken), compratorToken);
						break;
					case STRING:
						thisQuery = new infinityRangeQuery<string>(compareAttribute, finalValueToken, compratorToken);
						break;
				}
			}
			queryVec.push_back(thisQuery);
			if (inputSQLStream.eof())
			{
				break;
			}
			else
			{
				inputSQLStream >> nextToken;
				if (nextToken != "and")
				{
					break;
				}
			}
		}
		dbAPI.deleteFrom(tableName, queryVec);
	}
	else
	{
		dbAPI.deleteFrom(tableName, {});
	}
}
