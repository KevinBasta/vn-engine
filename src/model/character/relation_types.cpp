
#include "relation_types.h"
#include "id.h"

#include <string>
#include <string_view>
#include <iostream>
#include <unordered_map>
#include <exception>

std::unordered_map<RelationTypes::RelationName, RelationTypes::RelationId> RelationTypes::relationTypes{};

void RelationTypes::addRelationType(std::string relationName) {
	relationTypes.insert({ relationName, IdGenerator<RelationTypes>::getId() });
}

void RelationTypes::addRelationType(std::string relationName, int id) {
	relationTypes.insert({ relationName, id });
}

/**
 * Returns relation id if relation type found.
 * Raises an exception otherwise.
 */
int RelationTypes::getRelationId(std::string relationName) {
	// PI (potential improvement) with: https://en.cppreference.com/w/cpp/container/unordered_map/find to pass string_view
	std::unordered_map<RelationName, RelationId>::const_iterator relationType{ relationTypes.find(relationName) };
	
	if (relationType != relationTypes.end()) {
		return relationType->second;
	}

	throw std::invalid_argument("Unknown relation name");
}

void RelationTypes::print() {
	std::cout << "Relation Types:" << std::endl;

	for (auto [key, value] : relationTypes) {
		std::cout << "\t name - " << key << "    \t id -   " << value << std::endl;
	}
}
