
#ifndef CHAPTER_H
#define CHAPTER_H

#include "graph.h"

class Chapter {
private:
	Graph m_graph{};

public:
	Chapter() {

	}

	Graph& getGraph() { return m_graph; }


};

#endif // CHAPTER_H
