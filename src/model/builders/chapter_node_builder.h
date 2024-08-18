#ifndef VN_CHAPTER_NODE_BUILDER_H
#define VN_CHAPTER_NODE_BUILDER_H

#include "chapter_node.h"
#include "chapter_node_types.h"

#include "linkable_builder.h"

class ChapterNodeBuilder : public LinkableBuilder {
private: 
	ChapterNode* m_nodeDerived{};

public:
	ChapterNodeBuilder() :
		LinkableBuilder{ nullptr },
		m_nodeDerived{ nullptr }
	{
		ChapterNode* node { new ChapterNode };

		m_linkableObject = node;
		m_nodeDerived = node;
	}

	ChapterNodeBuilder(std::string text) :
		LinkableBuilder{ nullptr },
		m_nodeDerived{ nullptr }
	{
		ChapterNode* node{ new ChapterNode(text) };

		m_linkableObject = node;
		m_nodeDerived = node;
	}

	ChapterNodeBuilder(ChapterNode* node) :
		LinkableBuilder{ node },
		m_nodeDerived{ node } 
	{
	}

	ChapterNode* get() {
		return m_nodeDerived;
	}

public:
	//
	// Loading and saving node model files
	//
	void save();
	void load();


public:
	// 
	// Action adding interface
	//

	template <class T>
	void addStep(int stepIndex, std::unordered_map <int, T> object) {
		
	}







private:
	void insertAction(int stepIndex, ChapterNodeActionType stepType) {
		if (!m_nodeDerived) { return; }
		if (stepIndex < 0) { return; }


		//for (int i{ m_nodeDerived->m_steps.size() - 1 }; i < stepIndex; i++) {
			//insertStep()
		//}

	}

	void removeAction(int stepIndex, ChapterNodeActionType stepType) {

	}

	void insertStep(int stepIndex) {

	}

	void removeStep(int stepIndex) {

	}

};

#endif // VN_CHAPTER_NODE_BUILDER_H