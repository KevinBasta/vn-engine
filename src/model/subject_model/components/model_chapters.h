#ifndef VN_MODEL_CHAPTERS_H
#define VN_MODEL_CHAPTERS_H

#include "chapter.h"
#include "chapter_builder.h"
#include "chapter_node.h"
#include "chapter_node_builder.h"
#include "chapter_node_types.h"
#include "engine_chapter_manager.h"
#include "engine_node_manager.h"

#include <memory>
#include <vector>
#include <unordered_map>

class ModelSubject;
class ModelCommonInterface;
class ModelRuntimeInterface;
class ModelEngineInterface;

class ModelChapters {
private:
	friend class ModelSubject;
	friend class ModelCommonInterface;
	friend class ModelRuntimeInterface;
	friend class ModelEngineInterface;

	EngineNodeManager m_nodes{};
	EngineChapterManager m_chapters{};

	void createChapterOne() {

		// Can inital all nodes as strays then mark the head node as not a stray with a member variable?
		Chapter* chapterOne = new Chapter{};

		ChapterNode* head{ new ChapterNode("head node") };

		ChapterNode* oneone{ new ChapterNode("one one node") };
		ChapterNode* onetwo{ new ChapterNode("one two node") };

		ChapterNode* twoone{ new ChapterNode("two one node") };
		ChapterNode* twotwo{ new ChapterNode("two two node") };

		ChapterNode* threeone{ new ChapterNode("three one node") };

		ChapterNodeBuilder{ head }.link(oneone);
		ChapterNodeBuilder{ head }.setStep<ActionTextLine>(1, ActionTextLine{ 1, L"builder test hello!" });
		ChapterNodeBuilder{ head }.moveStep<ActionTextOverrideSpeaker>(2, 1);
		//ChapterNodeBuilder{ head }.removeStep<ActionTextLine>(1);
		ChapterNodeBuilder{ head }.link(onetwo);
		ChapterNodeBuilder{ oneone }.link(twoone);
		ChapterNodeBuilder{ oneone }.link(twotwo);
		ChapterNodeBuilder{ oneone }.link(threeone);
		ChapterNodeBuilder{ twoone }.link(threeone);

		ChapterNodeBuilder{ oneone }.unlink(threeone);
		ChapterNodeBuilder{ oneone }.unlink(threeone);


		//chapterOneChild4->addChild(chapterOneChild5);
		// chapterOneHead->addChild(chapterOneHead); TODO: unhandled case yet cycle
		ChapterBuilder{ chapterOne }.setHeadNodeId(head);
		ChapterBuilder{ chapterOne }.setName(L"Garu 3 node tree");

		m_chapters.add(chapterOne);
		m_nodes.add(head);
		m_nodes.add(oneone);
		m_nodes.add(onetwo);
		m_nodes.add(twoone);
		m_nodes.add(twotwo);
		m_nodes.add(threeone);

		Chapter* chapterTwo = new Chapter{};
		ChapterNode* fourone = ChapterNodeBuilder{ "four one node" }.get();
		m_nodes.add(fourone);
		

		ChapterBuilder{ chapterTwo }.setHeadNodeId(fourone);
		ChapterBuilder{ chapterTwo }.setName(L"Garu 1 node tree");

		ChapterBuilder{ chapterOne }.link(chapterTwo);

		m_chapters.add(chapterTwo);
		
		Chapter* chapterThree = new Chapter{};
		ChapterBuilder{ chapterThree }.setName(L"chapter 3");
		Chapter* chapterFour = new Chapter{};
		ChapterBuilder{ chapterFour }.setName(L"chapter 4");
		
		ChapterBuilder{ chapterOne }.link(chapterThree);
		ChapterBuilder{ chapterOne }.link(chapterFour);

		m_chapters.add(chapterThree);
		m_chapters.add(chapterFour);


		//std::cout << chapterOneGraph << std::endl;
	}

	void loadChildChapters() {

	}

	void loadChildNodes() {

	}

	void deallocLastNode() {
		// after step off a node and a new node is displayed
		// it doesn't need to be in memory anymore so it can be deallocated
		// in runtime, the next set of children can be loaded
		// all on different threads
		// flag system to say if something is being loaded
	}

	Chapter* getChapterById(id chapterId) {
		return m_chapters.get(chapterId);
	}

	const Chapter* getHeadChapter() {
		return m_chapters.get(1);
	}

	Node* getNodeById(id nodeId) {
		return m_nodes.get(nodeId);
	}


};


#endif // VN_MODEL_CHAPTERS_H