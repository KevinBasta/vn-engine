#ifndef VN_MODEL_CHAPTERS_H
#define VN_MODEL_CHAPTERS_H

#include "chapter.h"
#include "chapter_builder.h"
#include "chapter_node.h"
#include "chapter_node_builder.h"
#include "chapter_node_types.h"

#include <memory>
#include <vector>
#include <unordered_map>

class ModelSubject;

class ModelChapters {
private:
	friend class ModelSubject;

	std::unordered_map<id, std::unique_ptr<ChapterNode>> m_nodes{};
	std::unordered_map<id, std::unique_ptr<Chapter>> m_chapters{};
	std::list<Chapter*> m_chapterOrder{}; // TODO: remove, chapter class will be graph/tree

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

		m_chapters.insert(std::pair{ chapterOne->getId(), std::unique_ptr<Chapter>(chapterOne)});
		m_nodes[head->getId()] = std::unique_ptr<ChapterNode>(head);
		m_nodes[oneone->getId()] = std::unique_ptr<ChapterNode>(oneone);
		m_nodes[onetwo->getId()] = std::unique_ptr<ChapterNode>(onetwo);
		m_nodes[twoone->getId()] = std::unique_ptr<ChapterNode>(twoone);
		m_nodes[twotwo->getId()] = std::unique_ptr<ChapterNode>(twotwo);
		m_nodes[threeone->getId()] = std::unique_ptr<ChapterNode>(threeone);
		
		Chapter* chapterTwo = new Chapter{};
		ChapterNode* fourone = ChapterNodeBuilder{ "four one node" }.get();
		m_nodes[fourone->getId()] = std::unique_ptr<ChapterNode>(fourone);
		ChapterBuilder{ chapterTwo }.setHeadNodeId(fourone);

		ChapterBuilder{ chapterOne }.link(chapterTwo);

		m_chapters.insert(std::pair{ chapterTwo->getId(), std::unique_ptr<Chapter>(chapterTwo)});

		//std::cout << chapterOneGraph << std::endl;
	}

	Chapter* getChapterById(id chapterId) {
		// TODO: Load from model file if not loaded

		// TODO: this is temp for testing
		auto iter{ m_chapters.find(chapterId) };

		if (iter != m_chapters.end()) {
			return iter->second.get();
		} 
		
		return nullptr;
	}

	Node* getNodeById(id nodeId) {
		auto iter{ m_nodes.find(nodeId) };

		if (iter == m_nodes.end()) {
			// TODO: attempt to read the file
			// BUT should already covered by prefetcher on a diff thread,
			// so maybe check a vector containing the nodes currently loading
			return nullptr;
		}
		else {
			// return a pointer to the node
			return iter->second.get();
		}
	}


};


#endif // VN_MODEL_CHAPTERS_H