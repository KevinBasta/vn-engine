
#ifndef RELATION_TYPES_H
#define RELATION_TYPES_H

#include <string>
#include <string_view>
#include <unordered_map>
#include <functional>

class RelationTypes {
private:
	using RelationName = std::string;
	using RelationId = int;

	static std::unordered_map<RelationName, RelationId> relationTypes;

public:
	static void addRelationType(std::string relationName);
	static void addRelationType(std::string relationName, int id);
	static int getRelationId(std::string relationName); // TODO: change to string_view
	static void print();
};

#endif // RELATION_TYPES_H
