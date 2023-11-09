// vn_engine.cpp : Defines the entry point for the application.
//

#include "vn_engine.h"
#include "json_lexer.h"

#include <iostream>
#include <string>
#include <filesystem>

int main()
{
	std::cout << "Hello CMake." << std::endl;
	
	// Set path of test files
	std::filesystem::path jsonTestDir = "C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\tests\\Json\\";
	//std::filesystem::path filepath = std::filesystem::absolute(jsonpath);

	JsonLexer *test = new JsonLexer(jsonTestDir, "test1.json");

	return 0;
}
