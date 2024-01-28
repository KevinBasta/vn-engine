
#include "id.h"
#include "relation.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>



std::unordered_map<std::string, int> RelationTypes::relationTypes{};

void RelationTypes::addRelationType(std::string relationName) {
	relationTypes.insert({ relationName, IdGenerator::getId() });
}

void RelationTypes::print() {
	std::cout << "Relation Types:" << std::endl;

	for (auto [key, value] : RelationTypes::relationTypes) {
		std::cout << "\t name - " << key << "\t id -   " << value << std::endl;
	}
}




Relations::Relations() : m_relationships{} {

};

std::ostream& operator<<(std::ostream& out, Relations relation) {
	std::cout << "Relation:" << std::endl;

	return out;
}