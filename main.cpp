#include <iostream>
using std::cin;
using std::cout;
using std::string;
using std::getline;
#include <fstream>
using std::ifstream;
#include <vector>
using std::vector;

#include "src/parser.h"
#include "src/utils.h"


int main(int argc, char const * argv[])
{
	if (argc > 1) // execute file
	{
		parseFile(argv[1]);
	}
	else // command line mode
	{
		commandOperation();
	}
	return 0;
}
