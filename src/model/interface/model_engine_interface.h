
#ifndef MODEL_INTERFACE_ENGINE_H
#define MODEL_INTERFACE_ENGINE_H

#include "model_subject.h"
#include "model_common_interface.h"
#include "model_relation_types.h"

class ModelEngineInterface : public ModelCommonInterface {
public:
	using TextureStoreMap = ModelTextures::TextureStoreMap;
	static TextureStoreMap& getTextureStoreMap() {
		ModelSubject* model{ ModelSubject::validateInstance() };

		return model->m_modelTextures.m_textureStores;
	}

	using CharacterMap = ModelCharacters::CharacterMap;
	static CharacterMap& getCharacterMap() {
		ModelSubject* model{ ModelSubject::validateInstance() };

		return model->m_modelCharacters.m_characters;
	}

	using RelationsMap = ModelRelations::RelationsMap;
	static RelationsMap& getRelationsMap() {
		ModelSubject* model{ ModelSubject::validateInstance() };
		if (model == nullptr) { throw "model not found"; }
		return model->m_modelRelations.m_baseRelations;
	}

	using RelationTypeMap = ModelRelationTypes::TypeIdToNameMap;
	static RelationTypeMap& getRelationTypesMap() {
		ModelSubject* model{ ModelSubject::validateInstance() };
		if (model == nullptr) { throw "model not found"; }
		return model->m_modelRelationTypes.m_relationTypeIdToName;
	}

	static void addRelationType(std::string relationName) {
		ModelSubject* model{ ModelSubject::validateInstance() };
		
		model->m_modelRelationTypes.addRelationType(relationName);
	}

	/*static bool removeRelationType(id relationId) {
		ModelSubject* model{ ModelSubject::validateInstance() };
		if (model == nullptr) { return false; }

		RelationTypeMap* map{ model->m_modelRelationTypes.getTypeIdToNameMap() };
		if (map == nullptr) { return false; }

		auto iter{ map->find(relationId) };
		if (iter == map->end()) { return false; }

		map->erase(iter);

		return true;
	}*/

};

#endif // MODEL_INTERFACE_ENGINE_H