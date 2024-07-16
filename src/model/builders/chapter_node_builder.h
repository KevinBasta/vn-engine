#ifndef VN_CHAPTER_NODE_BUILDER_H
#define VN_CHAPTER_NODE_BUILDER_H

#include "chapter_node.h"
#include "chapter_node_types.h"

class ChapterNodeBuilder {
private: 
	ChapterNode* m_node{};

public:
	ChapterNodeBuilder() : m_node{ new ChapterNode } {}
	ChapterNodeBuilder(ChapterNode* node) : m_node{ node } {}

private:
	void insertAction(int stepIndex, ChapterNodeActionType stepType) {
		if (!m_node) { return; }
		if (stepIndex < 0) { return; }


		for (int i{ m_node->m_steps.size() - 1 }; i < stepIndex; i++) {
			//insertStep()
		}

	}

	void removeAction(int stepIndex, ChapterNodeActionType stepType) {

	}

	void insertStep(int stepIndex) {

	}



	void removeStep(int stepIndex) {

	}

};

#endif // VN_CHAPTER_NODE_BUILDER_H