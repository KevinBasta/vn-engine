
#include <memory>
#include <iostream>
#include <string>
#include <vector>

class character {
public:
	std::string m_name{};

	character(std::string name) : m_name{ name } {
		std::cout << "constructed" << std::endl;
	}

	~character() {
		std::cout << "destructed" << std::endl;
	}
};

// PROBLEM WAS THAT OPERATOR << TOOK CHARACTER BY VALUE
std::ostream& operator<<(std::ostream& out, character& c1) {
	std::cout << "character: " << c1.m_name << std::endl;

	return out;
}

std::vector<std::unique_ptr<character>> g_vec{};

void fillVec() {
	std::unique_ptr<character> c1{ std::make_unique<character>("test") };
	g_vec.push_back(std::move(c1));
}

int main() {
	fillVec();

	for (int i{ 0 }; i < g_vec.size(); i++) {
		character* test = g_vec[i].get();
		std::cout << *test << std::endl;
	}
}