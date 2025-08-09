#ifndef VN_MODEL_CHARACTERS_H
#define VN_MODEL_CHARACTERS_H

#include "character.h"
#include "character_builder.h"

#include <exception>

class ModelSubject;
class ModelCommonInterface;
class ModelRuntimeInterface;
class ModelEngineInterface;

class ModelCharacters {
private:
	friend class ModelSubject;
	friend class ModelCommonInterface;
	friend class ModelRuntimeInterface;
	friend class ModelEngineInterface;

	using CharacterMap = std::unordered_map<id, std::unique_ptr<Character>>;
	CharacterMap m_characters{};
	
	void initCharacters() {
		Character* garu = new Character();
		CharacterBuilder{ garu }.setName(L"Garu");

		CharacterBuilder brz{};
		brz.setName(L"Brazazaza");

		m_characters[garu->getId()] = std::unique_ptr<Character>{ garu };
		m_characters[brz.get()->getId()] = std::unique_ptr<Character>{ brz.get() };
	}

	Character* getCharacterById(int id) {
		try {
			return m_characters.at(id).get();
		}
		catch (std::out_of_range) {
			return nullptr;
		}
	}
};

#endif // VN_MODEL_CHARACTERS_H
