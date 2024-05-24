
#include "relation_types.h"
#include "id.h"

#include <string>
#include <iostream>
#include <unordered_map>

void RelationTypes::addRelationType(std::string relationName) {
	relationTypes.insert({ relationName, IdGenerator::getId() });
}

/**
 * Place relation id in outId if relation type found.
 * Return true if relation found, false otherwise.
 */
bool RelationTypes::getRelationId(std::string relationName, int& outId) {
	// PI (potential improvement) with: https://en.cppreference.com/w/cpp/container/unordered_map/find to pass string_view
	std::unordered_map<RelationName, RelationId>::const_iterator relationType = RelationTypes::relationTypes.find(relationName);
	
	if (relationType != RelationTypes::relationTypes.end()) {
		outId = relationType->second;
		return true;
	}
	else {
		return false;
	}
}

void RelationTypes::print() {
	std::cout << "Relation Types:" << std::endl;

	for (auto [key, value] : RelationTypes::relationTypes) {
		std::cout << "\t name - " << key << "    \t id -   " << value << std::endl;
	}
}
