
#ifndef MODEL_INTERFACE_RUNTIME_H
#define MODEL_INTERFACE_RUNTIME_H

#include "model_subject.h"
#include "model_common_interface.h"

class ModelRuntimeInterface : public ModelCommonInterface {
public:
	// Characters
	static Character* getCharacterById(int id) {
		ModelSubject* model{ ModelSubject::validateInstance() };

		return model->m_modelCharacters.getCharacterById(id);
	}

	// Textures
	static void loadTexture(const TextureIdentifier& textureId) {
		ModelSubject* model{ ModelSubject::validateInstance() };

		model->m_modelTextures.loadTexture(textureId);
	}

	// Relations
	using RelationsMap = ModelRelations::RelationsMap;
	static const RelationsMap& getBaseRelations() {
		ModelSubject* model{ ModelSubject::validateInstance() };

		return model->m_modelRelations.m_baseRelations;
	}
};

#endif // MODEL_INTERFACE_RUNTIME_H