#ifndef VN_TEXTURE_MANAGER_H
#define VN_TEXTURE_MANAGER_H

#include "texture.h"

#include <memory>
#include <unordered_map>

// A class to keep track of all the textures, init them, delete them, return them, etc..
// other classes to keep just an identifier of the texture as an int or perhaps string

class TextureManager {
private:
	static std::unique_ptr<TextureManager> m_instance;

	static void checkInstance() {
		if (m_instance.get() == nullptr) {
			m_instance = std::make_unique<TextureManager>();
		}
	}

private:
	using textureId = int;
	using idToTextureMap = std::unordered_map<textureId, std::unique_ptr<Texture2D>>;

	idToTextureMap m_textureMap{};

public:
	static int registerTexture(std::string path) {
		checkInstance();
		
		TextureManager* manager{ m_instance.get() };

		std::unique_ptr<Texture2D> texture{ std::make_unique<Texture2D>(path) };
		textureId id{ texture.get()->vnId() };

		manager->m_textureMap[id] = std::move(texture);

		return id;
	}

	static Texture2D* getTexture(int id) {
		checkInstance();

		TextureManager* manager{ m_instance.get() };

		idToTextureMap::iterator iter{ manager->m_textureMap.find(id) };

		if (iter == manager->m_textureMap.end()) {
			return nullptr;
		}

		return iter->second.get();
	}
};

#endif // VN_TEXTURE_MANAGER_H
