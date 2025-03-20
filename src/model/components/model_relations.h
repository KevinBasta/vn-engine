#ifndef VN_MODEL_RELATIONS_H
#define VN_MODEL_RELATIONS_H

#include "id.h"
#include "model_relation_types.h"

#include "relations.h"
#include "relations_builder.h"

#include <memory>
#include <unordered_map>

class ModelSubject;
class ModelCommonInterface;
class ModelRuntimeInterface;
class ModelEngineInterface;

class ModelRelations {
private:
	friend class ModelSubject;
	friend class ModelCommonInterface;
	friend class ModelRuntimeInterface;
	friend class ModelEngineInterface;

	using RelationsMap = std::unordered_map<id, std::unique_ptr<Relations>>;
	
	RelationsMap m_baseRelations{};

	void initBaseRelations() {
		id garuId{ 1 };
		id brzId{ 2 };

		RelationsBuilder garuRelations{ };
		RelationsBuilder brzRelations{ };

		int friendshipId = 1;
		int respectId = 2;
		int hatredId = 3;

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
};

#endif // VN_MODEL_RELATIONS_H
