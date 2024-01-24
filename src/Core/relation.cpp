
#include "relation.h"

#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::string, int>> Relation::relationTypes{};

void Relation::addRelationType(std::string relationName) {
	relationTypes.push_back(std::pair(relationName, static_cast<int>(relationTypes.size())));
}