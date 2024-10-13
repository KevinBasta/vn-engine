#ifndef VN_ENGINE_CHAPTER_GRAPH_H
#define VN_ENGINE_CHAPTER_GRAPH_H

#include "engine_graph.h"

#include "linkable.h"
#include "state_subject.h"
#include "model_subject.h"

// TODO: ALL ERROR HANDLING 
class VnEngineChapterGraph : public VnEngineGraph {
private:
	StateSubject* m_stateSubject{ nullptr };

public:
	VnEngineChapterGraph(StateSubject* stateSubject) :
		VnEngineGraph{ "chpaterGraph.json" },
		m_stateSubject{ stateSubject } 
	{
	}

	std::string getGraphName() {
		return "CHAPTERS_GRAPH_EDITOR";
	}

	id getLinkableHeadId() {
		return ModelSubject::getFirstChapterId();
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

	id getCurrentStateLinkable() {
		if (m_stateSubject != nullptr) {
			return m_stateSubject->getChapterId();
		}

		return getLinkableHeadId();
	}

	void setCurrentStateToLinkable(id linkableId) {
		m_stateSubject->goToChapterId(linkableId);
	}
};

#endif // VN_ENGINE_CHAPTER_GRAPH_H