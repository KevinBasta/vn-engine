
#ifndef MODEL_INTERFACE_ENGINE_H
#define MODEL_INTERFACE_ENGINE_H

#include "model_subject.h"
#include "model_common_interface.h"

class ModelEngineInterface : public ModelCommonInterface {
public:
	using TextureStoreMap = ModelTextures::TextureStoreMap;
	static TextureStoreMap& getTextureStoreMap() {
		ModelSubject* model{ ModelSubject::validateInstance() };

		return model->m_modelTextures.m_textureStores;
	}

	using CharacterMap = ModelCharacters::CharacterMap;
	static const CharacterMap& getCharacterMap() {
		ModelSubject* model{ ModelSubject::validateInstance() };

		return model->m_modelCharacters.m_characters;
	}
};

#endif // MODEL_INTERFACE_ENGINE_H