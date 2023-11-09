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
	std::string data;
	std::filesystem::path filePath;
	std::vector<std::string> tokens;
};
