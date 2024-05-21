#ifndef MODEL_SUBJECT_H
#define MODEL_SUBJECT_H

#include "subject.h"

#include "character.h"
#include "node.h"
#include "node_children.h"
#include "chapter_node.h"
#include "graph.h"
#include "chapter.h"

#include "relation.h"
#include "relation_types.h"
#include "state_subject.h"

#include <vector>
#include <memory>

class ModelSubject : Subject {
public:
	StateSubject* m_stateSubject{};
	std::vector<std::unique_ptr<Chapter>>	m_chapters{};
	std::vector<std::unique_ptr<Character>> m_characters{};
	Texture2D backgroundTexture; // TEMP
	// arr telling which chapters loaded, func to ckeck and load
public:
	ModelSubject(): backgroundTexture{ "C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\assets\\test.jpg", TextureType::BACKGROUND }  
	{

	}

	void attatchStateSubject(StateSubject* stateSubject) {
		m_stateSubject = stateSubject;
		m_stateSubject->setTempTexture(&(m_characters[0].get()->getTextures()[0]));
		m_stateSubject->setTempBackground(&backgroundTexture);

		m_chapters[0].get()->attatchControllingObject(m_stateSubject);
	}

	void forwardProgress() {
		m_chapters[0].get()->forwardProgress();
	}

	void createChapterOne() {
		Chapter* chapterOne = new Chapter{};

		Graph& chapterOneGraph{ chapterOne->getGraph() };

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
		// chapterOneHead->addChild(chapterOneHead); unhandled case yet

		//chapterOneHead->addCharacter(m_characters[0].get());

		chapterOneGraph.setHeadNode(chapterOneHead);

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

	void initRelations() {
		RelationTypes::addRelationType("friendship");
		RelationTypes::addRelationType("respect");
		RelationTypes::addRelationType("hatred");
		// RelationTypes::print();

		Character* garu = m_characters[0].get();
		Relations& garuRelations = (*garu).getRelationsObject();

		Character* brz = m_characters[1].get();
		Relations& brzRelations = (*brz).getRelationsObject();

		int friendshipId = 0; RelationTypes::getRelationId("friendship", friendshipId);
		int respectId = 0;    RelationTypes::getRelationId("respect", respectId);
		int hatredId = 0;     RelationTypes::getRelationId("hatred", hatredId);

		int garuId = garu->getId();
		int brzId = brz->getId();

		//std::cout << garuId << brzId << friendshipId << respectId << hatredId << std::endl;

		garuRelations.addCharacterRelation(brzId, friendshipId, 1);
		//std::cout << garuRelations;
		garuRelations.addCharacterRelation(brzId, hatredId, 20);
		//std::cout << garuRelations;
		garuRelations.addCharacterRelation(brzId, hatredId, 2);
		//std::cout << garuRelations;
		garuRelations.addCharacterRelation(brzId, hatredId, -10);
		//std::cout << garuRelations;

		brzRelations.addCharacterRelation(garuId, respectId, 1);
	}
};

#endif // MODEL_SUBJECT_H
