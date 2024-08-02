#ifndef VN_CHAPTER_ITERATOR_H
#define VN_CHAPTER_ITERATOR_H

#include "id.h"
#include "chapter.h"

#include "node.h"
#include "node_runner.h"

#include "model_subject.h"

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
	ModelSubject*	m_model{ nullptr };
	
	id m_curNodeId{};
	Node* m_curNodePtr{ nullptr };
	NodeRunner m_nodeRunner{ nullptr };

	id m_curChapterId{};
	Chapter* m_curChapterPtr{ nullptr };
	
public:
	// TODO: iterator with no chapter id input, for new game cases (perhaps can still pass but pass the first chapter id)
	ChapterIterator(ModelSubject* model, int chapterId) :
		m_model{ model },
		m_curChapterId{ chapterId }
	{
		// Only for loading the chapter if it's not loaded, can move to first .step
		// can make a special function to request the loading of it
		//ModelSubject::getChapterById(chapterId);
	}

	bool goToNextNode(StateSubject* stateSubject);

	ChapterState step(StateSubject* stateSubject);
	//ChapterState stepBack
};


#endif // VN_CHAPTER_ITERATOR_H
