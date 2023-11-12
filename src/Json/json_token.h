
#ifndef JSON_TOKEN_H
#define JSON_TOKEN_H

#include <iostream>
#include <string>

enum tokenType {
	// Single-character tokens
	LEFT_BRACE, RIGHT_BRACE, COLON, COMMA,

	// Literals
	STRING, NUMBER,
	
	// Keywords
	// TRUE, FALSE, 

	END_OF_FILE,
};

class JsonToken {
private:
	tokenType type;
	std::string lexeme;
	int line;

public:
	JsonToken(tokenType enumType, std::string parsedString, int lineNumber);
	friend std::ostream& operator<<(std::ostream& os, const JsonToken& token);
};

#endif // JSON_TOKEN_H