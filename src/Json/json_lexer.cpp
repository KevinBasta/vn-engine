#include "json_lexer.h"

#include "json_token.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>


JsonLexer::JsonLexer(std::filesystem::path filedir, std::string filename) {
	filepath = (filedir.string()).append(filename);
	
	start	= 0;
	current = 0;
	line	= 1;
}

void JsonLexer::lex() {
	filestream.open(filepath, std::ios::in);

	// try catch?
	scanTokens();

	filestream.close();

	std::vector<JsonToken>::iterator iter = tokens.begin();
	for (; iter < tokens.end(); iter++) {
		std::cout << *iter << std::endl;
	}
}

void JsonLexer::scanTokens() {

	char ch;

	while (filestream.get(ch)) {
		std::cout << ch << std::endl;
		start = current;
		dataBuffer += ch;
		switch(ch) {
			case '{':
				addToken(LEFT_BRACE);
				break;
			case '}':
				addToken(RIGHT_BRACE);
				break;
			case ':':
				addToken(COLON);
				break;
			case ',':
				addToken(COMMA);
				break;

			case '"':
				scanString();
				break;
			

			case ' ':
			case '\r':
			case '\t':
				break;

			case '\n':
				line++;
				break;
			
			default:
				if (isDigit(ch)) {
					scanNumber();
				}
				break;
		}

		dataBuffer.erase();
	}


	tokens.push_back(JsonToken(END_OF_FILE, "", line));
}


void JsonLexer::addToken(tokenType type) {
	tokens.push_back(JsonToken(type, dataBuffer, line));
}


void JsonLexer::scanString() {
	while (filestream.peek() && filestream.peek() != '"') {
		char ch;
		filestream.get(ch);
		dataBuffer += ch;
	
		if (ch == '\n') {
			// raise an error, strings can't span lines
			std::cout << "STRING HAS NEW LINE IN IT" << std::endl;
		}
	}

	if (!filestream.peek()) {
		// raise an error, unterminated string
		std::cout << "STRING IS UNTERMINATED" << std::endl;
	}

	char ch;
	filestream.get(ch);
	dataBuffer += ch;

	addToken(STRING);
}


bool JsonLexer::isDigit(char ch) {
	// need to check if the std::isdigit can be used
	return ch >= '0' && ch <= '9';
}

void JsonLexer::scanNumber() {
	char ch{};
	
	while (filestream.peek() && isDigit(filestream.peek())) {
		filestream.get(ch);
		dataBuffer += ch;
	}


	// REFACTOR TO IGNORE THE DECIMAL IF THERE IS NO NUMBER AFTER IT
	if (filestream.peek() == '.') {
		filestream.get(ch);
		dataBuffer += ch;

		if (filestream.peek() && isDigit(filestream.peek())) {
			while (filestream.peek() && isDigit(filestream.peek())) {
				filestream.get(ch);
				dataBuffer += ch;
			}
		} 
		else {
			// Fatal error, number ending on a decimal
			std::cout << "INVALID DECIMAL, NO FOLLOWING NUMBERS" << std::endl;
		}
	}

	if (ch == '\n') {
		// raise an error, strings can't span lines
		std::cout << "NUMBER HAS NEW LINE IN IT BEFORE COMMA" << std::endl;
	}

	addToken(NUMBER);
}

//static void checkNewLine(char ch) {
//
//}

/*std::istream& JsonLexer::scanToken() {
	//do {
	//	result = scanToken();
	//} while (result);

	char ch;
	std::istream& result = filestream.get(ch);

	while (result) {
		std::cout << ch << std::endl;
		result = filestream.get(ch);
	}

	return result;
}*/


