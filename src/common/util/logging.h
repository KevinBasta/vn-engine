#ifndef VN_LOG_H
#define VN_LOG_H

#include <string>
#include <iostream>
#include <fstream>

// need a way to set and remove warnings, such as 
// removed texture index from texture store used
// should be set on discovery and unset on user-resolution
class EngineLog {
	// singleton?

public:
	//static EngineLog& dbg(std::string& str) {

	//	return nullptr;
	//}

	static void writeOut() {

	}

	static void resetLogs() {

	}
	
	static void stateError(std::string err) {

	}

	static void modelError(std::string err) {

	}

	static void engineError(std::string err) {

	}

	static void engineWarning(std::string err) {

	}

	EngineLog* operator<<(std::string& str) {
		std::cout << str << std::endl;
		return this;
	}

	//void registerWarning() {}
	//void unregisterWarning() {}
};

#endif // VN_LOG_H
