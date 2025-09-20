
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

	static Character* getCharacterById(id id) {
		ModelSubject* model{ ModelSubject::validateInstance() };

		auto character{ model->m_modelCharacters.m_characters.find(id) };
		if (character == model->m_modelCharacters.m_characters.end()) {
			return nullptr;
		}

		return character->second.get();
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

	using ChapterMap = EngineChapterManager::ChapterMap;
	static ChapterMap& getChaptersMap() {
		ModelSubject* model{ ModelSubject::validateInstance() };
		if (model == nullptr) { throw "model not found"; }
		return model->m_modelChapters.m_chapters.getChapters();
	}

	static void addChapter(Chapter* chapter) {
		ModelSubject* model{ ModelSubject::validateInstance() };
		if (model == nullptr) { throw "model not found"; }
		return model->m_modelChapters.m_chapters.add(chapter);
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