
#ifndef RELATION_TYPES_H
#define RELATION_TYPES_H

#include <string>
#include <unordered_map>

class RelationTypes {
private:
	using RelationName = std::string;
	using RelationId = int;

	std::unordered_map<RelationName, RelationId> relationTypes{};

public:
	void addRelationType(std::string relationName);
	bool getRelationId(std::string relationName, int& outId);
	void print();
};

#endif // RELATION_TYPES_H
