#include "parser.h"
#ifndef _UTILS_H_
#define _UTILS_H_

template<typename T>
vector<T> 
split(const T & str, const T & delimiters) {
    vector<T> v;
    typename T::size_type start = 0;
    auto pos = str.find_first_of(delimiters, start);
    while(pos != T::npos) 
    {
        if(pos != start) // ignore empty tokens
            v.emplace_back(str, start, pos - start);
        start = pos + 1;
        pos = str.find_first_of(delimiters, start);
    }
    if(start < str.length()) // ignore trailing delimiter
        v.emplace_back(str, start, str.length() - start); // add what's left of the string
    return v;
}


void parseFile(string filename)
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
			parser p(sqlCommand);
		}
	}
	else
	{
		cout << "could not open the file " << filename << '\n';
	}
};

void commandOperation()
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
		parser p(sqlCommand);
	}
};

#endif