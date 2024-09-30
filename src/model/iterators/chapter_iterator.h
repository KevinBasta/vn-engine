#ifndef VN_CHAPTER_ITERATOR_H
#define VN_CHAPTER_ITERATOR_H

#include "id.h"
#include "chapter.h"

#include "node.h"
#include "node_runner.h"

#include "model_subject.h"

#include <optional>
#include <iostream>

class StateSubject;

enum class ChapterState {
	CHAPTER_STEP,
	CHAPTERS_END,
	CHAPTER_ERR
};

inline std::ostream& operator<<(std::ostream& out, const ChapterState value) {
	switch (value)
	{
	case (ChapterState::CHAPTER_STEP):
		out << "CHAPTER_STEP";
		break;
	case (ChapterState::CHAPTERS_END):
		out << "CHAPTERS_END";
		break;
	case (ChapterState::CHAPTER_ERR):
		out << "CHAPTER_ERR";
		break;
	default:
		break;
	}

	return out;
}

class ChapterIterator {
private:	
	std::optional<id> m_chapterId{};
	Chapter*		  m_chapterPtr{ nullptr };
	
	std::optional<id>	m_nodeId{};
	Node*				m_nodePtr{ nullptr };
	NodeRunner			m_nodeRunner{ nullptr };

	bool m_goToNextNode{ false };

	void defaultChapterId();
	void updateChapterPtr();
	void defaultNodeId();
	void updateNodePtr();


	bool advanceChapter(StateSubject* stateSubject);
	bool advanceNode(StateSubject* stateSubject);

public:
	// TODO: iterator with no chapter id input, for new game cases (perhaps can still pass but pass the first chapter id)
	ChapterIterator():
		m_chapterId{ 1 }
	{
		 // To request from model
	}
	
	ChapterIterator(id chapterId) :
		m_chapterId{ chapterId }
	{
		// Only for loading the chapter if it's not loaded, can move to first .step
		// can make a special function to request the loading of it
		//ModelSubject::getChapterById(chapterId);
	}

	ChapterIterator(id chapterId, id nodeId) :
		m_chapterId{ chapterId },
		m_nodeId{ nodeId }
	{
		
	}

	bool goToNextNode(StateSubject* stateSubject);
	ChapterState step(StateSubject* stateSubject);

	//ChapterState stepBack

public:
	// Engine operations:
	id getChapterId() {
		return m_chapterId.has_value() ? m_chapterId.value() : 0;
	}

	id getNodeId() {
		return m_nodeId.has_value() ? m_nodeId.value() : 0;
	}

	index getCurrentStepIndex() {
		//return m_nodeRunner.
		return 0;
	}
};


#endif // VN_CHAPTER_ITERATOR_H
