
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
	tokenType type{};
	std::string lexeme{};
	int line{};

public:
	JsonToken(tokenType enumType, std::string parsedString, int lineNumber);
	friend std::ostream& operator<<(std::ostream& os, const JsonToken& token);
};



//void testJson() {
//	// Set path of test files
//	std::filesystem::path jsonTestDir = "C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\tests\\Json\\";
//	//std::filesystem::path filepath = std::filesystem::absolute(jsonpath);
//
//	Timer t;
//	t.resetp();
//
//	JsonLexer test{ jsonTestDir, "test1.json" };
//	test.lex();
//
//	t.elapsedp();
//}


#endif // JSON_TOKEN_H