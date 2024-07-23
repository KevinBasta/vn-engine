#ifndef VN_CHAPTER_NODE_BUILDER_H
#define VN_CHAPTER_NODE_BUILDER_H

#include "chapter_node.h"
#include "chapter_node_types.h"

#include "node_builder.h"

class ChapterNodeBuilder : NodeBuilder {
private: 
	ChapterNode* m_nodeDerived{};

public:
	ChapterNodeBuilder() :
		NodeBuilder{ nullptr },
		m_nodeDerived{ nullptr }
	{
		ChapterNode* node { new ChapterNode };

		m_nodeBase = node;
		m_nodeDerived = node;
	}

	ChapterNodeBuilder(ChapterNode* node) :
		NodeBuilder{ node },
		m_nodeDerived{ node } 
	{
	
	}

private:
	void insertAction(int stepIndex, ChapterNodeActionType stepType) {
		if (!m_nodeDerived) { return; }
		if (stepIndex < 0) { return; }


		for (int i{ m_nodeDerived->m_steps.size() - 1 }; i < stepIndex; i++) {
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