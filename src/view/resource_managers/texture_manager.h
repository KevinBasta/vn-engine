#ifndef VN_TEXTURE_MANAGER_H
#define VN_TEXTURE_MANAGER_H

#include "id.h"
#include "texture.h"
#include "chapter_node_types.h"

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
	using idToTextureMap = std::unordered_map<TextureIdentifier, std::unique_ptr<Texture2D>, TextureIdentifierHasher>;

	idToTextureMap m_textureMap{};

public:
	static void registerTexture(TextureIdentifier textureIdentifier, std::string path) {
		checkInstance();
		
		TextureManager* manager{ m_instance.get() };

		// NOTE: construction of a Texture2D is expensive. Can cause slowdowns on main thread.
		// TODO: assess only calling texture manager on a separate thread and doing the texture loading ahead of time.
		std::unique_ptr<Texture2D> texture{ std::make_unique<Texture2D>(path) };

		manager->m_textureMap[textureIdentifier] = std::move(texture);
	}

	static void unregisterTexture(TextureIdentifier textureIdentifier) {
		checkInstance();

		TextureManager* manager{ m_instance.get() };

		idToTextureMap::iterator iter{ manager->m_textureMap.find(textureIdentifier) };

		if (iter == manager->m_textureMap.end()) {
			return;
		}

		manager->m_textureMap.erase(iter);
	}

	static Texture2D* getTexture(TextureIdentifier textureIdentifier) {
		checkInstance();

		TextureManager* manager{ m_instance.get() };

		idToTextureMap::iterator iter{ manager->m_textureMap.find(textureIdentifier) };

		if (iter == manager->m_textureMap.end()) {
			return nullptr;
		}

		return iter->second.get();
	}
};

#endif // VN_TEXTURE_MANAGER_H
