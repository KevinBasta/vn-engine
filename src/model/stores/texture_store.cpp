#include "texture_store.h"

#include <string>
#include <unordered_map>
#include <stdexcept>

TextureStore::textureId TextureStore::getTextureIdByIndex(int textureIndex) {
	if (textureIndex < 0 || textureIndex >= m_texturePaths.size()) {
		throw std::out_of_range{ "Texture index out of range" };
	}

	textureId returnId{ 0 };
	std::string& texturePath{ m_texturePaths[textureIndex] };

	runtimeTextureMap::iterator textureEntry{ m_textureLookup.find(texturePath) };

	if (textureEntry != m_textureLookup.end()) {
		returnId = textureEntry->second;
	}
	else {
		textureId id{ TextureManager::registerTexture(texturePath) };

		m_textureLookup[texturePath] = id;
		returnId = id;
	}

	return returnId;
}
