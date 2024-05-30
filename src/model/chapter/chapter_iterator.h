#ifndef VN_CHAPTER_ITERATOR_H
#define VN_CHAPTER_ITERATOR_H

#include "chapter.h"
#include "graph.h"
#include "graph_iterator.h"
#include "model_subject.h"

class ChapterIterator {
private:
	int m_currentChapterIndex{ 0 };
	Chapter* m_currentChapter{ nullptr };
	Graph* m_currentGraph{ nullptr };

public:
	ChapterIterator(Chapter* chapter) :
		m_currentChapter{ chapter }
	{
		
	}



};


#endif // VN_CHAPTER_ITERATOR_H
