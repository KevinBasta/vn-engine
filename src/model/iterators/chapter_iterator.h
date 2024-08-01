#ifndef VN_CHAPTER_ITERATOR_H
#define VN_CHAPTER_ITERATOR_H

#include "id.h"
#include "chapter.h"

#include "node.h"
#include "node_runner.h"

#include "model_subject.h"

class StateSubject;

enum class ChapterState {
	CHAPTER_STEP,
	CHAPTERS_END,
	CHAPTER_ERR
};

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
		ModelSubject::getChapterById(chapterId);
	}

	bool goToNextNode(StateSubject* stateSubject);

	ChapterState step(StateSubject* stateSubject) {
		if (m_curChapterPtr == nullptr) {
			m_curChapterPtr = ModelSubject::getChapterById(m_curChapterId);
			if (m_curChapterPtr == nullptr) {
				// TODO: throw exception instead??
				return ChapterState::CHAPTER_ERR;
			} 
		}

		if (m_curNodePtr == nullptr) {
			m_curNodeId = m_curChapterPtr->getHeadNodeId();
			m_curNodePtr = ModelSubject::getNodeById(m_curChapterId);

			if (m_curNodePtr == nullptr) {
				// TODO: throw exception instead??
				return ChapterState::CHAPTER_ERR;
			}

			m_nodeRunner = m_curNodePtr->iter();
		}

		NodeState state = m_nodeRunner.step(stateSubject);

		if (state == NodeState::NODE_END) {
			bool inNextNode{ goToNextNode(stateSubject) };
			if (!inNextNode) {
				return ChapterState::CHAPTERS_END;
			}
		}

		return ChapterState::CHAPTER_STEP;
	}
};


#endif // VN_CHAPTER_ITERATOR_H
