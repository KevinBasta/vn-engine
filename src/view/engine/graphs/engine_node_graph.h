#ifndef VN_ENGINE_NODE_GRAPH_H
#define VN_ENGINE_NODE_GRAPH_H

#include "engine_graph.h"

#include "linkable.h"
#include "state_subject.h"
#include "model_subject.h"

#include <locale>
#include <codecvt>
#include <string>

class VnEngineNodeGraph : public VnEngineGraph {
private:
	StateSubject* m_stateSubject{ nullptr };

public:
	VnEngineNodeGraph(StateSubject* stateSubject) :
		VnEngineGraph{ "nodeGraph.json" },
		m_stateSubject{ stateSubject }
	{

	}

	std::string getGraphName() {
		return "NODES_GRAPH_EDITOR";
	}

	id getLinkableHeadId() {
		return ModelSubject::getChapterById(m_stateSubject->getChapterId())->getHeadNodeId();
	}

	Linkable* getLinkableById(id linkableId) {
		return static_cast<Linkable*>(ModelSubject::getNodeById(linkableId));
	}

	const Linkable* getLinkableHead() {
		if (m_stateSubject == nullptr) {
			return nullptr;
		}

		return static_cast<const Linkable*>(ModelSubject::getNodeById(ModelSubject::getChapterById(m_stateSubject->getChapterId())->getHeadNodeId()));
	}

	std::wstring getLinkableName(id linkableId) {
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wide = converter.from_bytes(ModelSubject::getNodeById(linkableId)->m_temp);

		return wide;
	}

	id getCurrentStateLinkable() {
		if (m_stateSubject != nullptr) {
			return m_stateSubject->getNodeId();
		}

		return getLinkableHeadId();
	}

	void setCurrentStateToLinkable(id linkableId) {
		m_stateSubject->goToNodeId(linkableId);
	}
};

#endif // VN_ENGINE_NODE_GRAPH_H
