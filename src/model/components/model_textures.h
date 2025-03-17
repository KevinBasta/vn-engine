#ifndef VN_MODEL_TEXTURES_H
#define VN_MODEL_TEXTURES_H

#include "texture_store.h"
#include "texture_store_builder.h"

#include <memory>
#include <unordered_map>

#define TEMP_BACKGROUND_TEXTURE VN_BASE_PATH"/test/assets/test.jpg"
#define TEMP_SPRITE_TEXTURE		VN_BASE_PATH"/test/assets/garu_outline.png"

class ModelSubject;
class ModelCommonInterface;
class ModelRuntimeInterface;
class ModelEngineInterface;

class ModelTextures {
private:
	friend class ModelSubject;
	friend class ModelCommonInterface;
	friend class ModelRuntimeInterface;
	friend class ModelEngineInterface;

	using TextureStoreMap = std::unordered_map<id, std::unique_ptr<TextureStore>>;
	TextureStoreMap m_textureStores{};

	void initTextureStores() {
		TextureStoreBuilder garuStore{};
		garuStore.addTexture(TEMP_SPRITE_TEXTURE);
		garuStore.setName("Garu Sprites");

		m_textureStores[garuStore.get()->getId()] = std::unique_ptr<TextureStore>{ garuStore.get() };

		TextureStoreBuilder backgroundStore{};
		backgroundStore.addTexture(TEMP_BACKGROUND_TEXTURE);
		backgroundStore.setName("chapter one backgrounds");

		m_textureStores[backgroundStore.get()->getId()] = std::unique_ptr<TextureStore>{ backgroundStore.get() };
	}

	void loadTexture(const TextureIdentifier& textureId) {
		auto iter{ m_textureStores.find(textureId.textureStoreId) };

		if (iter != m_textureStores.end()) {
			iter->second.get()->loadTexture(textureId.textureIndex);
		}
		else {
			// TODO: throw exception
		}
	}
};


#endif // VN_MODEL_TEXTURES_H
