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
	std::wstring m_name{};

	friend class TextureStoreBuilder;
	
	using compiletimePathVector = std::vector<std::optional<std::string>>;
	compiletimePathVector m_texturePaths{};

	using runtimeTextureSet = std::set<index>;
	runtimeTextureSet m_loadedTextures{};

public:
	id getId() { return m_id;  }
	std::wstring_view getName() { return m_name; }
	void loadTexture(index textureIndex);
};

#endif // VN_TEXTURE_STORE_H
