// vn_engine.cpp : Defines the entry point for the application.
//

#include "vn_engine.h"
#include "json_lexer.h"
#include "timer.h"

#include <iostream>
#include <string>
#include <filesystem>

#include "runtime_characters.h"
#include "runtime_relations.h"

void testJson() {
	// Set path of test files
	std::filesystem::path jsonTestDir = "C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\tests\\Json\\";
	//std::filesystem::path filepath = std::filesystem::absolute(jsonpath);

	Timer t;
	t.resetp();

	JsonLexer test{ jsonTestDir, "test1.json" };
	test.lex();

	t.elapsedp();
}

void testCore() {
	initCharacters();
	initRelations();


	/*for (auto i : g_characters) {
		std::cout << i;
	}*/
}


int main()
{
	std::cout << "Hello CMake." << std::endl;
	
	testCore();

	return 0;
}
