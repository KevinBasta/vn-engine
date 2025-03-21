#ifndef VN_TEXTURE_STORE_H
#define VN_TEXTURE_STORE_H

#include "id.h"
#include "index.h"
#include "texture_manager.h"

#include <string>
#include <string_view>
#include <unordered_map>
#include <set>
#include <optional>
#include <stdexcept>
#include <filesystem>

class TextureStoreBuilder;

/**
 * A class to contain groups of textures
 */
class TextureStore {
public:
	TextureStore() : m_id{ IdGenerator<TextureStore>::getId() } {}
	TextureStore(id id) : m_id{ id } {}

private:
	id m_id{};
	std::string m_name{};

	friend class TextureStoreBuilder;
	
	using compiletimePathVector = std::vector<std::optional<std::string>>;
	compiletimePathVector m_texturePaths{};

	using runtimeTextureSet = std::set<index>;
	runtimeTextureSet m_loadedTextures{};

public:
	const id getId() { return m_id;  }
	const std::string& getName() { return m_name; }

	// Return 0 to the size of the texture paths [0, max + 1]
	std::pair<int, int> getTexturesRange() { return { 0, m_texturePaths.size() }; }
	void loadTexture(index textureIndex);
};

#endif // VN_TEXTURE_STORE_H
