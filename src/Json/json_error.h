
#ifndef JSON_ERROR_H
#define JSON_ERROR_H

#include <string>

class JsonError {
public:
	void error(int line, std::string message);
	void report(int line, std::string location, std::string message);
};

#endif // JSON_ERROR_H