
#include "json_error.h"

#include <iostream>

void JsonError::error(int line, std::string message) {
	report(line, "", message);
}

void JsonError::report(int line, std::string location, std::string message) {
	std::cout << "[line " << line << "] Error" << location << ": " << message << std::endl;
}