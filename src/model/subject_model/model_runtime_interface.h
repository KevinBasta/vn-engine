
#ifndef MODEL_RUNTIME_INTERFACE_H
#define MODEL_RUNTIME_INTERFACE_H

#include "model_subject.h"

class ModelRuntimeInterface {
public:

	// Characters
	static Character* getCharacterById(int id) {
		ModelSubject* model{ ModelSubject::validateInstance() };

		return model->m_modelCharacters.getCharacterById(id);
	}

	// Textures
	static void loadTexture(TextureIdentifier& textureId) {
		ModelSubject* model{ ModelSubject::validateInstance() };

		model->m_modelTextures.loadTexture(textureId);
	}

	// Relations
	static const std::unordered_map<id, std::unique_ptr<Relations>>& getBaseRelations() {
		ModelSubject* model{ ModelSubject::validateInstance() };

		return model->m_modelRelations.m_baseRelations;
	}
};


#endif // MODEL_RUNTIME_INTERFACE_H