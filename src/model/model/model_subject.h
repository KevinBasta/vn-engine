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

#include <vector>
#include <memory>


#define TEMP_BACKGROUND "C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\assets\\test.jpg"


// Circular dependancy resolutions:
class ChapterIterator;

class ModelSubject : public Subject {
public:
	// This class is only for creating the chapters and characters. The current would actually be stored in the stat
	std::vector<std::unique_ptr<Chapter>>	m_chapters{};
	std::vector<std::unique_ptr<Character>> m_characters{};
	std::vector<std::unique_ptr<Texture2D>> m_backgrounds{};

	std::list<Chapter*> m_chapterOrder{};

	Chapter* getChapterByOrderIndex(int orderIndex) {
		return m_chapters[0].get(); // TODO: reason for current cycle
	}

	ChapterIterator iter(int chapterIndex);

	// arr telling which chapters loaded, func to ckeck and load
public:
	ModelSubject()  
	{
	}

	void createChapterOne() {
		Chapter* chapterOne = new Chapter{};

		Graph* chapterOneGraph{ new Graph() };

		ChapterNode* chapterOneHead{ new ChapterNode("the head node!") };
		ChapterNode* chapterOneChild1{ new ChapterNode("level 1 child node 1!") };
		ChapterNode* chapterOneChild2{ new ChapterNode("level 1 child node 2!") };
		ChapterNode* chapterOneChild3{ new ChapterNode("level 1 child node 3!") };

		ChapterNode* chapterOneChild4{ new ChapterNode("level 2 child node 1!") };

		ChapterNode* chapterOneChild5{ new ChapterNode("level 2 child node 2!") };

		chapterOneHead->addChild(chapterOneChild1);
		chapterOneHead->addChild(chapterOneChild2);
		chapterOneHead->addChild(chapterOneChild3);

		chapterOneChild1->addChild(chapterOneChild4);
		chapterOneChild1->addChild(chapterOneChild2);
		chapterOneChild2->addChild(chapterOneChild5);

		//chapterOneChild4->addChild(chapterOneChild5);
		// chapterOneHead->addChild(chapterOneHead); TODO: unhandled case yet cycle

		//chapterOneHead->addCharacter(m_characters[0].get());

		chapterOneGraph->setHeadNode(chapterOneHead);

		chapterOne->setGraph(chapterOneGraph);

		m_chapters.push_back(std::unique_ptr<Chapter>(chapterOne));
		//std::cout << chapterOneGraph << std::endl;
	}

	void initCharacters() {
		std::unique_ptr<Character> garu{ std::make_unique<Character>("garu") };
		std::unique_ptr<Character> brz{ std::make_unique<Character>("brazazaza") };

		garu.get()->addTexture("C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\assets\\garu_outline.png");

		m_characters.push_back(std::move(garu));
		m_characters.push_back(std::move(brz));
	}

	void initBackgrounds() {
		std::unique_ptr<Texture2D> background_0{ std::make_unique<Texture2D>(TEMP_BACKGROUND) };
		m_backgrounds.push_back(std::move(background_0));
	}

	//void initRelations() {
	//	RelationTypes::addRelationType("friendship");
	//	RelationTypes::addRelationType("respect");
	//	RelationTypes::addRelationType("hatred");
	//	// RelationTypes::print();

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
