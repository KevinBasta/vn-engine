#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class JsonLexer {
public:
	JsonLexer(std::string filename);
	void lex();

private:
	std::ifstream filestream;
	std::vector<std::string> tokens;
};
