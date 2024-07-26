#ifndef VN_TEXTURE_STORE_H
#define VN_TEXTURE_STORE_H

#include "id.h"
#include "texture_manager.h"

#include <string>
#include <string_view>
#include <unordered_map>
#include <stdexcept>
#include <filesystem>

/**
 * A class to contain groups of textures
 */
class TextureStore {
public:
	using textureId = id;

private:
	using runtimeTextureMap = std::unordered_map<std::string_view, textureId>;

	std::vector<std::string> m_texturePaths{};
	runtimeTextureMap m_textureLookup{};

public:
	textureId getTextureIdByIndex(int textureIndex);
	void addTexture(std::string path) {
		// TODO: Validate the file path given is readable
		if (!std::filesystem::exists(path)) {
			return;
		}

		m_texturePaths.push_back(path);
	}
};

#endif // VN_TEXTURE_STORE_H
