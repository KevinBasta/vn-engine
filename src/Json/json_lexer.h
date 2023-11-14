
#ifndef JSON_LEXER_H
#define JSON_LEXER_H

#include "json_token.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

class JsonLexer {
public:
	JsonLexer(std::filesystem::path filepath, std::string filename);
	void lex();

private:
	std::string filepath;
	std::fstream filestream;
	
	int start;
	int current;
	int line;
	
	std::string dataBuffer;
	std::vector<JsonToken> tokens;
	
	void scanToken();
	void scanTokens();
	void scanString();
	void scanNumber();
	bool isDigit(char ch);
	void addToken(tokenType type);
};

#endif // JSON_LEXER_H