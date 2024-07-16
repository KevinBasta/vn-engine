#ifndef VN_TEXTURE_MANAGER_H
#define VN_TEXTURE_MANAGER_H

#include "texture.h"

#include <memory>
#include <unordered_map>

// A class to keep track of all the textures, init them, delete them, return them, etc..
// other classes to keep just an identifier of the texture as an int or perhaps string

class TextureManager {
private:
	using textureId = int;
	using idToTextureMap = std::unordered_map<textureId, std::unique_ptr<Texture2D>>;

	idToTextureMap m_textureMap{};

public:
	int registerTexture(const char* path) {
		std::unique_ptr<Texture2D> texture{ std::make_unique<Texture2D>(path) };
		textureId id{ texture.get()->vnId() };

		m_textureMap[id] = std::move(texture);

		return id;
	}

	Texture2D* getTexture(int id) {
		idToTextureMap::iterator iter{ m_textureMap.find(id) };

		if (iter == m_textureMap.end()) {
			return nullptr;
		}

		return iter->second.get();
	}
};

#endif // VN_TEXTURE_MANAGER_H
