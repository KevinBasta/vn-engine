#include "id.h"
#include "index.h"
#include "texture_store.h"

#include <string>
#include <optional>
#include <unordered_map>
#include <stdexcept>

void TextureStore::loadTexture(index textureIndex) {
	if (textureIndex < 0 || textureIndex >= m_texturePaths.size()) {
		throw std::out_of_range{ "Texture index out of range" };
	}

	if (m_texturePaths[textureIndex].has_value() == false) {
		throw std::range_error{ "Texture at index is empty" };
	}

	runtimeTextureSet::iterator textureLoaded{ m_loadedTextures.find(textureIndex) };

	if (textureLoaded == m_loadedTextures.end()) {
		std::string& texturePath{ m_texturePaths[textureIndex].value()};
		TextureManager::registerTexture({m_id, textureIndex}, texturePath);
	}
}
