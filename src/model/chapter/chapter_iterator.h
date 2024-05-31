#ifndef VN_CHAPTER_ITERATOR_H
#define VN_CHAPTER_ITERATOR_H

#include "chapter.h"
#include "graph.h"
#include "graph_iterator.h"
#include "model_subject.h"

class StateSubject;

enum class ChapterState {
	CHAPTER_STEP,
	CHAPTERS_END
};


class ChapterIterator {
private:
	ModelSubject* m_modelSubject{ nullptr };
	StateSubject* m_stateSubject{ nullptr };

	int m_currentChapterIndex{ 0 };
	Chapter* m_currentChapter{ nullptr };
	Graph* m_currentGraph{ nullptr };
	GraphIterator m_graphIterator{ nullptr };

public:
	ChapterIterator(StateSubject* state, ModelSubject* model, int chapterIndex) :
		m_modelSubject{ model }	
	{
		if (m_modelSubject) {
			m_currentChapter = model->getChapterByOrderIndex(chapterIndex);
		
			if (m_currentChapter) {
				m_currentGraph = m_currentChapter->getGraph();
				m_graphIterator = m_currentGraph->iterator();
				m_graphIterator.attatchStateSubject(m_stateSubject);
			}
		}
	}

	ChapterState step() {
		GraphState state = m_graphIterator.step();

		return ChapterState::CHAPTER_STEP;
	}



};


#endif // VN_CHAPTER_ITERATOR_H
