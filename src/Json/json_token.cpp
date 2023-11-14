
#include "json_token.h"

#include <iostream>

JsonToken::JsonToken(tokenType enumType, std::string parsedString, int lineNumber) {
	type = enumType;
	lexeme = parsedString;
	line = lineNumber;
}

std::ostream& operator<<(std::ostream& os, const JsonToken& token) {
	std::cout << "type: " << token.type << ", string: " << token.lexeme << ", line #" << token.line;
	return os;
}