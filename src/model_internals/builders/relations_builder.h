#ifndef VN_CHARACTER_RELATIONS_BUILDER_H
#define VN_CHARACTER_RELATIONS_BUILDER_H

#include "id.h"
#include "relations.h"
#include "chapter_node_types.h"

class RelationsBuilder {
private:
	Relations* m_relations{ nullptr };

public:
	RelationsBuilder() : m_relations{ new Relations{} } {}
	RelationsBuilder(Relations* relations) : m_relations{ relations } {}
	~RelationsBuilder() {}

	bool setRelationValue(id relationWithId, id relationTypeId, int relationAmount) {
		if (m_relations == nullptr) { return false; }

		bool success{ m_relations->modifyRelationWith(relationWithId, relationTypeId, RelationModification::SET, relationAmount) };

		return success;
	}

	Relations* get() {
		return m_relations;
	}

public:
	//
	// Loading and saving node model and save files
	//
	void save();
	void load();
};

#endif // VN_CHARACTER_RELATIONS_BUILDER_H
