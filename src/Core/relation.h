
#ifndef RELATION_H
#define RELATION_H

#include "id.h"

#include <vector>
#include <unordered_map>
#include <utility>
#include <string>

/*
 * A generalized data class that contians some statuses (e.g. affection)
 * towards the main character. Can be generalized to include for all other characters?
 */

class Relation {
private:
	// relation name, relation id
	static std::vector<std::pair<std::string, int>> relationTypes;

	// maybe a weak pointer to a shared pointer that is the character?
	// character id: list(<relation id, relation level>)
	std::unordered_map<int, std::vector<std::pair<int, int>>> relationLevel{};

public: 
	Relation() : relationLevel{} {
	
	};

	static void addRelationType(std::string relationName);
};

#endif // RELATION_H