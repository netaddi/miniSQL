// #include "src/stdafx.h"
#include "src/parser.h"

int main(int argc, char const * argv[])
{
	Parser p;
	if (argc > 1) // execute file
	{
		p.parseFile(argv[1]);
	}
	else // command line mode
	{
		p.commandOperation();
	}
	return 0;
}
