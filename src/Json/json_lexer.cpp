#include "json_lexer.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>


JsonLexer::JsonLexer(std::filesystem::path filedir, std::string filename) {
	std::string filepath = (filedir.string()).append(filename);
	
	std::fstream filestream;
	filestream.open(filepath, std::ios::in);

	/*
	char ch;
	filestream.read(&ch, 1);
	std::cout << ch << std::endl;
	*/

	std::string line;

	while (std::getline(filestream, line)) {
		std::cout << line << std::endl;
	}
}

void JsonLexer::lex() {

}
