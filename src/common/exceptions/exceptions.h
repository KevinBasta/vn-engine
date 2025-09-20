#ifndef VN_EXCEPTIONS_H
#define VN_EXCEPTIONS_H

#include <exception>

#include <string>

class InvalidArgumentException : public std::exception {
private:
	std::string m_message{};

public:
	InvalidArgumentException(const std::string& message) :
		m_message{ message } 
	{
	}

	const char* what() const noexcept override {
		return m_message.c_str();
	}
};




#endif // VN_EXCEPTIONS_H