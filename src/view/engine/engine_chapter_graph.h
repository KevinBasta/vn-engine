#ifndef VN_ENGINE_CHAPTER_GRAPH_H
#define VN_ENGINE_CHAPTER_GRAPH_H

#include "engine_graph.h"

#include "linkable.h"
#include "state_subject.h"
#include "model_subject.h"

class VnEngineChapterGraph : public VnEngineGraph {
private:
	StateSubject* m_stateSubject{ nullptr };

public:
	VnEngineChapterGraph(StateSubject* stateSubject) :
		VnEngineGraph{ "chpaterGraph.json" },
		m_stateSubject{ stateSubject } 
	{
	}

	Linkable* getLinkableById(id linkableId) {
		return static_cast<Linkable*>(ModelSubject::getChapterById(linkableId));
	}
	
	const Linkable* getLinkableHead() {
		return static_cast<const Linkable*>(ModelSubject::getHeadChapter());
	}

	std::wstring getLinkableName(id linkableId) {
		return ModelSubject::getChapterById(linkableId)->getName();
	}
};

#endif // VN_ENGINE_CHAPTER_GRAPH_H