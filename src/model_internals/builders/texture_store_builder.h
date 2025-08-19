#ifndef VN_TEXTURE_STORE_BUILDER_H
#define VN_TEXTURE_STORE_BUILDER_H

#include "texture_store.h"
#include "texture_manager.h"

#include <string>
#include <optional>
#include <filesystem>

class TextureStoreBuilder {
private:
	TextureStore* m_textureStore{ nullptr };

public:
	TextureStoreBuilder() : m_textureStore{ new TextureStore{} } {}
	TextureStoreBuilder(TextureStore* textureStore) : m_textureStore{ textureStore } {}
	~TextureStoreBuilder() {}

	TextureStore* get() {
		return m_textureStore;
	}

private:
	void removeTextureFromLoadedSetAndTextureManager(index pathIndex) {
		auto iter = m_textureStore->m_loadedTextures.find(pathIndex);

		if (iter != m_textureStore->m_loadedTextures.end()) {
			TextureManager::unregisterTexture({ m_textureStore->m_id, pathIndex });
			m_textureStore->m_loadedTextures.erase(pathIndex);
		}
	}

public:
	void setName(std::string name) {
		if (m_textureStore == nullptr) { return; }

		m_textureStore->m_name = name;
	}

	void addTexture(std::string path) {
		if (m_textureStore == nullptr) { return; }

		// TODO: Validate the file path given is readable
		if (!std::filesystem::exists(path)) {
			return;
		}

		m_textureStore->m_texturePaths.push_back(path);
	}

	void replaceTexture(index pathIndex, std::string path) {
		if (m_textureStore == nullptr) { return; }
		// TODO: resize the array instead of exititing to allow for savefile building
		if (pathIndex < 0 || pathIndex >= m_textureStore->m_texturePaths.size()) { return; }

		// TODO: Validate the file path given is readable
		if (!std::filesystem::exists(path)) { return; }

		removeTextureFromLoadedSetAndTextureManager(pathIndex);

		m_textureStore->m_texturePaths[pathIndex].emplace(path);
	}

	void removeTexture(index pathIndex) {
		if (m_textureStore == nullptr) { return; }
		if (pathIndex < 0 || pathIndex >= m_textureStore->m_texturePaths.size()) { return; }

		if (!m_textureStore->m_texturePaths[pathIndex].has_value()) { return; }
		
		removeTextureFromLoadedSetAndTextureManager(pathIndex);

		m_textureStore->m_texturePaths[pathIndex].reset();
	}

	TextureStore::compiletimePathVector& getTexturePathVector() {
		if (m_textureStore == nullptr) { throw "No Texture Store"; }

		return m_textureStore->m_texturePaths;
	}
};

#endif // VN_TEXTURE_STORE_BUILDER_H