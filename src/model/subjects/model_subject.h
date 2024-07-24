#ifndef MODEL_SUBJECT_H
#define MODEL_SUBJECT_H

#include "subject.h"

#include "texture.h"
#include "character.h"
#include "node.h"
#include "node_children.h"
#include "chapter_node.h"
#include "graph.h"
#include "chapter.h"

#include "relation.h"
#include "relation_types.h"

#include "node_builder.h"
#include "chapter_node_builder.h"
#include "character_builder.h"

#include <vector>
#include <memory>


#define TEMP_BACKGROUND_TEXTURE VN_BASE_PATH"/test/assets/test.jpg"
#define TEMP_SPRITE_TEXTURE		VN_BASE_PATH"/test/assets/garu_outline.png"

// Circular dependancy resolutions:
class ChapterIterator;

class ModelSubject : public Subject {
public:
	// This class is only for creating the chapters and characters. The current would actually be stored in the stat
	using ID = int;
	using characterMap = std::unordered_map<ID, std::unique_ptr<Character>> ;
	
	std::vector<std::unique_ptr<Chapter>>	m_chapters{};
	characterMap m_characters{};
	std::vector<std::unique_ptr<Texture2D>> m_backgrounds{};

	std::list<Chapter*> m_chapterOrder{};

	Chapter* getChapterByOrderIndex(int orderIndex) {
		return m_chapters[0].get(); // TODO: reason for current cycle
	}

	ChapterIterator iter(int chapterIndex);

	// arr telling which chapters loaded, func to ckeck and load
public:
	Texture2D* getBackgroundTexture(int index) {
		return m_backgrounds[index].get();
	}

	const characterMap& getCharacters() {
		return m_characters;
	}

	Character* getCharacterByID(int id) {
		return m_characters[id].get();
	}

public:
	ModelSubject()  
	{
	}

	void createChapterOne() {
		Chapter* chapterOne = new Chapter{};

		Graph* chapterOneGraph{ new Graph() };

		// Can inital all nodes as strays then mark the head node as not a stray with a member variable?
		ChapterNode* head{ new ChapterNode("head node") };

		ChapterNode* oneone{ new ChapterNode("one one node") };
		ChapterNode* onetwo{ new ChapterNode("one two node") };
		
		ChapterNode* twoone{ new ChapterNode("two one node") };
		ChapterNode* twotwo{ new ChapterNode("two two node") };

		ChapterNode* threeone{ new ChapterNode("three one node") };

		ChapterNodeBuilder{ head }.addChild(oneone);
		ChapterNodeBuilder{ head }.addChild(onetwo);
		ChapterNodeBuilder{ oneone }.addChild(twoone);
		ChapterNodeBuilder{ oneone }.addChild(twotwo);
		ChapterNodeBuilder{ oneone }.addChild(threeone);
		ChapterNodeBuilder{ twoone }.addChild(threeone);
		
		ChapterNodeBuilder{ oneone }.removeChild(threeone);
		ChapterNodeBuilder{ oneone }.removeChild(threeone);

		//chapterOneChild4->addChild(chapterOneChild5);
		// chapterOneHead->addChild(chapterOneHead); TODO: unhandled case yet cycle

		chapterOneGraph->setHeadNode(head);

		chapterOne->setGraph(chapterOneGraph);

		m_chapters.push_back(std::unique_ptr<Chapter>(chapterOne));
		//std::cout << chapterOneGraph << std::endl;
	}

	void initCharacters() {
		CharacterBuilder garu{};
		garu.setName(L"Garu");
		garu.addTexture(TEMP_SPRITE_TEXTURE);

		CharacterBuilder brz{};
		brz.setName(L"Brazazaza");

		m_characters[garu.get()->getId()] = std::unique_ptr<Character>{ garu.get() };
		m_characters[brz.get()->getId()] = std::unique_ptr<Character>{ brz.get() };
	}

	void initBackgrounds() {
		std::unique_ptr<Texture2D> background_0{ std::make_unique<Texture2D>(TEMP_BACKGROUND_TEXTURE) };
		m_backgrounds.push_back(std::move(background_0));
	}

	void initRelationTypes() {
		RelationTypes::addRelationType("friendship");
		RelationTypes::addRelationType("respect");
		RelationTypes::addRelationType("hatred");
		RelationTypes::print();
	}

	//void initRelations() {
	//	Character* garu = m_characters[0].get();
	//	Relations& garuRelations = (*garu).getRelationsObject();

	//	Character* brz = m_characters[1].get();
	//	Relations& brzRelations = (*brz).getRelationsObject();

	//	int friendshipId = 0; RelationTypes::getRelationId("friendship", friendshipId);
	//	int respectId = 0;    RelationTypes::getRelationId("respect", respectId);
	//	int hatredId = 0;     RelationTypes::getRelationId("hatred", hatredId);

	//	int garuId = garu->getId();
	//	int brzId = brz->getId();

	//	//std::cout << garuId << brzId << friendshipId << respectId << hatredId << std::endl;

	//	garuRelations.addCharacterRelation(brzId, friendshipId, 1);
	//	//std::cout << garuRelations;
	//	garuRelations.addCharacterRelation(brzId, hatredId, 20);
	//	//std::cout << garuRelations;
	//	garuRelations.addCharacterRelation(brzId, hatredId, 2);
	//	//std::cout << garuRelations;
	//	garuRelations.addCharacterRelation(brzId, hatredId, -10);
	//	//std::cout << garuRelations;

	//	brzRelations.addCharacterRelation(garuId, respectId, 1);
	//}
};

#endif // MODEL_SUBJECT_H
