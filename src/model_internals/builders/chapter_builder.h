#ifndef VN_CHAPTER_BUILDER_H
#define VN_CHAPTER_BUILDER_H

#include "id.h"
#include "node.h"
#include "chapter.h"
#include "linkable_builder.h"

#include <exception>

class ChapterBuilder : public LinkableBuilder {
private:
	Chapter* m_chapter{ nullptr };

	void validateInstance() {
		if (m_chapter == nullptr) {
			throw std::invalid_argument("Chapter Builder: null chapter pointer.");
		}
	}

public:
	ChapterBuilder() : 
		LinkableBuilder{ nullptr },
		m_chapter{ nullptr }
	{
		m_chapter = new Chapter{};
		m_linkableObject = m_chapter;
	}

	ChapterBuilder(Chapter* chapter) :
		LinkableBuilder{ chapter },
		m_chapter{ chapter }
	{
	}

public:
	//
	// Shared Engine and Save/Load Interface
	//
	void setName(std::wstring name) {
		validateInstance();

		m_chapter->m_name = name;
	}

	void setHeadNodeId(id headNodeId) {
		validateInstance();

		m_chapter->m_headNodeId = headNodeId;
	}

	void setHeadNodeId(Node* headNode) {
		validateInstance();

		if (headNode == nullptr) { 
			throw std::invalid_argument("Chapter Builder: null head node pointer.");
		}

		m_chapter->m_headNodeId = headNode->getId();
	}
};


#endif // VN_CHAPTER_BUILDER_H