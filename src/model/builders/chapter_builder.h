#ifndef VN_CHAPTER_BUILDER_H
#define VN_CHAPTER_BUILDER_H

#include "id.h"
#include "chapter.h"
#include "node.h"

class ChapterBuilder {
protected:
	Chapter* m_chapter{ nullptr };

private:
	void nullCheck() {
		if (m_chapter == nullptr) {
			//throw 
		}
	}

public:
	ChapterBuilder() = delete;

	ChapterBuilder(Chapter* chapter) :
		m_chapter{ chapter }
	{
	}

	void setHeadNodeId(id headNodeId) {
		nullCheck();

		m_chapter->m_headNodeId = headNodeId;
	}

	void setHeadNodeId(Node* headNode) {
		nullCheck();

		if (headNode == nullptr) { return; }


		m_chapter->m_headNodeId = headNode->getId();
	}
};


#endif // VN_CHAPTER_BUILDER_H