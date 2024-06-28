#ifndef VN_CHAPTER_ITERATOR_H
#define VN_CHAPTER_ITERATOR_H

#include "chapter.h"

#include "graph.h"
#include "graph_traverser.h"

#include "model_subject.h"


class StateSubject;

enum class ChapterState {
	CHAPTER_STEP,
	CHAPTERS_END
};

class ChapterIterator {
private:
	ModelSubject*	m_model{ nullptr };
	Chapter*		m_curChapter{ nullptr };
	int				m_curChapterIndex{ 0 };
	
	Graph*			m_curGraph{ nullptr };
	GraphTraverser	m_graphIter{ nullptr };

	// flase for fail, true for success
	bool initChapter(int chapterIndex) {
		if (!m_model) {
			return false;
		}

		m_curChapter = m_model->getChapterByOrderIndex(m_curChapterIndex);

		if (!m_curChapter) {
			return false;
		}

		m_curGraph = m_curChapter->getGraph();

		if (!m_curGraph) {
			return false;
		}

		m_graphIter = m_curGraph->iter();
	
		return true;
	}

public:
	ChapterIterator(ModelSubject* model, int chapterIndex) :
		m_model{ model },
		m_curChapterIndex{ chapterIndex }
	{
		initChapter(m_curChapterIndex);
	}

	ChapterState step(StateSubject* stateSubject) {
		// TODO: check graph iter containing a null graph
		GraphState state = m_graphIter.step(stateSubject);

		if (m_model && (state == GraphState::GRAPH_END)) {
			m_curChapterIndex++;
			
			bool nextChapterInit = initChapter(m_curChapterIndex);
			if (!nextChapterInit) {
				// Avoid future traversals
				m_curChapter = nullptr;
				m_curGraph = nullptr;
				m_graphIter = GraphTraverser(nullptr);
				
				return ChapterState::CHAPTERS_END;
			}
			else {
				// TODO: dangerous for possible infinite recursion
				return step(stateSubject);
			}
		}

		return ChapterState::CHAPTER_STEP;
	}

	ChapterState subStep(StateSubject* stateSubject) {
		GraphState state = m_graphIter.subStep(stateSubject);

	}
};


#endif // VN_CHAPTER_ITERATOR_H
