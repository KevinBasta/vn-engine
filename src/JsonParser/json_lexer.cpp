#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "json_lexer.h"

JsonLexer::JsonLexer(std::string filename) {
	JsonLexer::filestream.open(filename);
	char ch;
	JsonLexer::filestream.get(ch);

	std::cout << ch << std::endl;
}

void JsonLexer::lex() {
	std::string line;

	while (getline(JsonLexer::filestream, line)) {
		std::cout << line << std::endl;
	}
}
