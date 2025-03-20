#ifndef VN_MODEL_RELATION_TYPES_H
#define VN_MODEL_RELATION_TYPES_H

#include "id.h"

#include <unordered_map>
#include <string>

class ModelSubject;
class ModelCommonInterface;
class ModelRuntimeInterface;
class ModelEngineInterface;

class ModelRelationTypes {
private:
	friend class ModelSubject;
	friend class ModelCommonInterface;
	friend class ModelRuntimeInterface;
	friend class ModelEngineInterface;

	using TypeIdToNameMap = std::unordered_map<id, std::string>;

	TypeIdToNameMap m_relationTypeIdToName{};

	void init() {
		addRelationType("friendship");
		addRelationType("respect");
		addRelationType("hatred");
	}

	void addRelationType(std::string relationName) {
		m_relationTypeIdToName.insert({ IdGenerator<ModelRelationTypes>::getId(), relationName });
	}

	void addRelationType(std::string relationName, id id) {
		IdGenerator<ModelRelationTypes>::setIdToAfter(id);
		m_relationTypeIdToName.insert({ id, relationName });
	}

	TypeIdToNameMap* getTypeIdToNameMap() {
		return &m_relationTypeIdToName;
	}

	void print() {
		/*
		std::cout << "Relation Types:" << std::endl;

		for (auto [key, value] : relationTypes) {
			std::cout << "\t id - " << key << "    \t name -   " << value << std::endl;
		}
		*/
	}
};

#endif // VN_MODEL_RELATION_TYPES_H