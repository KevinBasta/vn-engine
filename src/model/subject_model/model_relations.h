#ifndef VN_MODEL_RELATIONS_H
#define VN_MODEL_RELATIONS_H

#include "id.h"
#include "relation_types.h"

#include "relations.h"
#include "relations_builder.h"

#include <memory>
#include <unordered_map>

class ModelSubject;
class ModelRuntimeInterface;
class ModelEngineInterface;

class ModelRelations {
private:
	friend class ModelSubject;
	friend class ModelRuntimeInterface;
	friend class ModelEngineInterface;

	std::unordered_map<id, std::unique_ptr<Relations>> m_baseRelations{};

	static void initRelationTypes() {
		RelationTypes::addRelationType("friendship");
		RelationTypes::addRelationType("respect");
		RelationTypes::addRelationType("hatred");
		RelationTypes::print();
	}

	void initBaseRelations() {
		initRelationTypes();
		id garuId{ 1 };
		id brzId{ 2 };

		RelationsBuilder garuRelations{ };
		RelationsBuilder brzRelations{ };

		int friendshipId = RelationTypes::getRelationId("friendship");
		int respectId = RelationTypes::getRelationId("respect");
		int hatredId = RelationTypes::getRelationId("hatred");

		garuRelations.setRelationValue(brzId, friendshipId, 1);
		garuRelations.setRelationValue(brzId, hatredId, 20);
		garuRelations.setRelationValue(brzId, hatredId, 2);
		garuRelations.setRelationValue(brzId, hatredId, -10);

		brzRelations.setRelationValue(garuId, respectId, 1);

		std::cout << *garuRelations.get();
		std::cout << *brzRelations.get();

		m_baseRelations[garuId] = std::unique_ptr<Relations>{ garuRelations.get() };
		m_baseRelations[brzId] = std::unique_ptr<Relations>{ brzRelations.get() };
	}

	const std::unordered_map<id, std::unique_ptr<Relations>>& getBaseRelations() {
		return m_baseRelations;
	}
};

#endif // VN_MODEL_RELATIONS_H
