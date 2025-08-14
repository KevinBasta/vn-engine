#ifndef VN_MODEL_CHAPTERS_H
#define VN_MODEL_CHAPTERS_H

#include "chapter.h"
#include "chapter_builder.h"
#include "node.h"
#include "node_builder.h"
#include "node_types.h"
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

		Node* head{ new Node("head node") };

		Node* oneone{ new Node("one one node") };
		Node* onetwo{ new Node("one two node") };

		Node* twoone{ new Node("two one node") };
		Node* twotwo{ new Node("two two node") };

		Node* threeone{ new Node("three one node") };

		NodeBuilder{ head }.link(oneone);
		NodeBuilder{ head }.setStep<ActionTextLine>(1, ActionTextLine{ false, 1, L"builder test hello!" });
		NodeBuilder{ head }.moveStep<ActionTextOverrides>(2, 1);
		//NodeBuilder{ head }.removeStep<ActionTextLine>(1);
		NodeBuilder{ head }.link(onetwo);
		NodeBuilder{ oneone }.link(twoone);
		NodeBuilder{ oneone }.link(twotwo);
		NodeBuilder{ oneone }.link(threeone);
		NodeBuilder{ twoone }.link(threeone);

		NodeBuilder{ oneone }.unlink(threeone);
		NodeBuilder{ oneone }.unlink(threeone);


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
		Node* fourone = NodeBuilder{ "four one node" }.get();
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
		// TODO: NEEDS TO LOAD FROM SQL
		return m_nodes.get(nodeId);
	}


};


#endif // VN_MODEL_CHAPTERS_H