
#ifndef RELATION_TYPES_H
#define RELATION_TYPES_H

#include <string>
#include <unordered_map>

class RelationTypes {
private:
	using RelationName = std::string;
	using RelationId = int;

	static std::unordered_map<RelationName, RelationId> relationTypes;

public:
	static void addRelationType(std::string relationName);
	static bool getRelationId(std::string relationName, int& outId);
	static void print();
};

#endif // RELATION_TYPES_H
