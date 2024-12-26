
#include "id.h"
#include "index.h"

#include "state_subject.h"
#include "model_engine_interface.h"

#include "node.h"
#include "chapter_node_builder.h"

#include "engine_timeline.h"

// TODO: null error checking on state subjct
class VnEngineStepTimeline : public VnEngineTimeline {
private:
	StateSubject* m_stateSubject{ nullptr };
	id m_previousNodeId{ 0 };

public:
	VnEngineStepTimeline(StateSubject* stateSubject) :
		VnEngineTimeline{ "stepTimeline.json" },
		m_stateSubject{ stateSubject }
	{ 
	}

protected:
	std::string getGraphName() {
		return "STEPS_TIMELINE";
	}

	std::pair<index, index> getTimelineRange() {
		id current{ m_stateSubject->getNodeId() };
		ChapterNode* node{ static_cast<ChapterNode*>(ModelCommonInterface::getNodeById(current)) };
		if (node == nullptr) { return { 0,0 }; }
		
		// TODO: the range starting from 1 depends on some internal chapterNode property. Perhaps certralize this.
		return { 0, node->getLastStepIndex() };
	}

	index getTimlineRangeMax() {
		id current{ m_stateSubject->getNodeId() };
		ChapterNode* node{ static_cast<ChapterNode*>(ModelCommonInterface::getNodeById(current)) };
		if (node == nullptr) { return 0; }

		return node->getLastStepIndex();
	}

	bool isCurrentlySelected(index rangeIndex) {
		index current{ m_stateSubject->getStepIndex() };

		return current == rangeIndex;
	}
	
	void setCurrentlySelectedToIndex(index rangeIndex) {
		m_stateSubject->goToStepIndex(rangeIndex);
	}

	bool shouldRedrawPositions() {
		id current{ m_stateSubject->getNodeId() };

		if (current != m_previousNodeId) {
			m_previousNodeId = current;
			return true;
		}

		return false;
	}

	bool handleDeletingNode(index rangeIndex) {
		id current{ m_stateSubject->getNodeId() };
		ChapterNode* node{ static_cast<ChapterNode*>(ModelCommonInterface::getNodeById(current)) };

		std::cout << "in handle deleting node" << std::endl;
		//ChapterNodeBuilder{ node }.removeStep(rangeIndex);

		return false;
	}

	ImColor getSelectionColor() {
		id current{ m_stateSubject->getNodeId() };
		ChapterNode* node{ static_cast<ChapterNode*>(ModelCommonInterface::getNodeById(current)) };
		
		if (m_stateSubject->inAutoAction()) {
			// TODO: perhaps it can start as green and over time as we get closer to
			// the end of the autoaction it can transition into the default colour
			return ImColor(58, 255, 46, 120);
		}
		
		return ImColor(128, 128, 128, 200);
	}
};