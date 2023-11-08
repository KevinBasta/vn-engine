// vn_engine.cpp : Defines the entry point for the application.
//

#include "vn_engine.h"
#include "json_lexer.h"

#include <string>

int main()
{
	std::cout << "Hello CMake." << std::endl;

	std::string filename = "JsonTests/test1.json";
	JsonLexer test(filename);
	test.lex();

	return 0;
}
