#ifndef VN_CHAPTER_NODE_BUILDER_H
#define VN_CHAPTER_NODE_BUILDER_H

#include "chapter_node.h"
#include "chapter_node_types.h"

#include "engine_drag_drop_payload.h"

#include "linkable_builder.h"

#include <vector>
#include <unordered_map>

class ChapterNodeBuilder : public LinkableBuilder {
private:
	ChapterNode* m_nodeDerived{};

public:
	ChapterNodeBuilder() :
		LinkableBuilder{ nullptr },
		m_nodeDerived{ nullptr }
	{
		ChapterNode* node{ new ChapterNode };

		m_linkableObject = node;
		m_nodeDerived = node;
	}

	ChapterNodeBuilder(std::string text) :
		LinkableBuilder{ nullptr },
		m_nodeDerived{ nullptr }
	{
		ChapterNode* node{ new ChapterNode(text) };

		m_linkableObject = node;
		m_nodeDerived = node;
	}

	ChapterNodeBuilder(ChapterNode* node) :
		LinkableBuilder{ node },
		m_nodeDerived{ node }
	{
	}

	ChapterNode* get() {
		return m_nodeDerived;
	}

public:
	//
	// Loading and saving node model files
	//
	void save();
	void load();


public:
	// 
	// Action adding interface
	//

	// access element similar to state subject variable access from node	
	template <class T>
	void setStep(int stepIndex, T object) {
		(m_nodeDerived->*(chapterNodeHelper<T>::handler))[stepIndex].clear();
		(m_nodeDerived->*(chapterNodeHelper<T>::handler))[stepIndex].push_back(object);
	}

	/*template <class T>
	void addToStep(int stepIndex, T object) {
		(m_nodeDerived->*(chapterNodeHelper<T>::handler))[stepIndex].push_back(object);
	}*/

	template <class T>
	bool removeStep(index stepIndex) {
		auto iter{ (m_nodeDerived->*(chapterNodeHelper<T>::handler)).find(stepIndex) };

		if (iter == (m_nodeDerived->*(chapterNodeHelper<T>::handler)).end()) {
			return false;
		}

		(m_nodeDerived->*(chapterNodeHelper<T>::handler)).erase(iter);
		return true;
	}

	template <class T>
	void moveStep(int oldStepIndex, int newStepIndex) {
		auto iter{ (m_nodeDerived->*(chapterNodeHelper<T>::handler)).find(oldStepIndex) };

		if (iter != (m_nodeDerived->*(chapterNodeHelper<T>::handler)).end()) {
			auto item{ (m_nodeDerived->*(chapterNodeHelper<T>::handler))[oldStepIndex] };
			(m_nodeDerived->*(chapterNodeHelper<T>::handler)).erase(iter);
			(m_nodeDerived->*(chapterNodeHelper<T>::handler))[newStepIndex] = item;
		}
	}

	template <class T>
	void replaceAction(index stepIndex, T obj) {
		// TODO: when replacing at step, validate that the step is in the step range?
		(m_nodeDerived->*(chapterNodeHelper<T>::handler))[stepIndex] = { obj };
	}

	template <class T>
	void addAction(index stepIndex, T obj) {
		// TODO: when adding to node, validate that the step is in the step range?
		(m_nodeDerived->*(chapterNodeHelper<T>::handler))[stepIndex].push_back(obj);
	}

	template <class T>
	bool moveAction(ActionDragDropPayload payload) {
		// Don't allow the source to be the destination because then the action gets deleted
		if (payload.m_sourceStepIndex == payload.m_destinationStepIndex) { return false; }

		auto sourceStepIter{ (m_nodeDerived->*(chapterNodeHelper<T>::handler)).find(payload.m_sourceStepIndex) };

		if (sourceStepIter != (m_nodeDerived->*(chapterNodeHelper<T>::handler)).end()) {
			// Assumes that payload only sets pick one if it is a vector (not single) type
			if (payload.m_pickOne) {
				// Move one of the actions inside the step for type T
				if (payload.m_replace) {
					(m_nodeDerived->*(chapterNodeHelper<T>::handler))[payload.m_destinationStepIndex].clear();
					(m_nodeDerived->*(chapterNodeHelper<T>::handler))[payload.m_destinationStepIndex].push_back(sourceStepIter->second.at(payload.m_souceStepPickOneIndex));
				}
				else {
					(m_nodeDerived->*(chapterNodeHelper<T>::handler))[payload.m_destinationStepIndex].push_back(sourceStepIter->second.at(payload.m_souceStepPickOneIndex));
				}
			}
			else {
				// Move all the actions inside the step for type T
				if (payload.m_replace) {
					(m_nodeDerived->*(chapterNodeHelper<T>::handler))[payload.m_destinationStepIndex] = sourceStepIter->second;
				}
				else {
					((m_nodeDerived->*(chapterNodeHelper<T>::handler))[payload.m_destinationStepIndex]).insert(
						(m_nodeDerived->*(chapterNodeHelper<T>::handler))[payload.m_destinationStepIndex].end(),
						sourceStepIter->second.begin(),
						sourceStepIter->second.end()
					);
				}
			}

			// Erase the entry from the source step
			(m_nodeDerived->*(chapterNodeHelper<T>::handler)).erase(sourceStepIter);
			return true;
		}

		//TODO: perhaps report error at this point
		return false;
	}

	template <class T>
	bool copyAction(ActionDragDropPayload payload) {
		auto sourceStepIter{ (m_nodeDerived->*(chapterNodeHelper<T>::handler)).find(payload.m_sourceStepIndex) };

		if (sourceStepIter != (m_nodeDerived->*(chapterNodeHelper<T>::handler)).end()) {
			// Assumes that payload only sets pick one if it is a vector (not single) type
			if (payload.m_pickOne) {
				// Move one of the actions inside the step for type T
				if (payload.m_replace) {
					(m_nodeDerived->*(chapterNodeHelper<T>::handler))[payload.m_destinationStepIndex].clear();
					(m_nodeDerived->*(chapterNodeHelper<T>::handler))[payload.m_destinationStepIndex].push_back(sourceStepIter->second.at(payload.m_souceStepPickOneIndex));
				}
				else {
					// TODO: could use more bounds checks on vector access?
					(m_nodeDerived->*(chapterNodeHelper<T>::handler))[payload.m_destinationStepIndex].push_back(sourceStepIter->second.at(payload.m_souceStepPickOneIndex));
				}
			}
			else {
				// Move all the actions inside the step for type T
				if (payload.m_replace) {
					(m_nodeDerived->*(chapterNodeHelper<T>::handler))[payload.m_destinationStepIndex] = sourceStepIter->second;
				}
				else {
					((m_nodeDerived->*(chapterNodeHelper<T>::handler))[payload.m_destinationStepIndex]).insert(
						(m_nodeDerived->*(chapterNodeHelper<T>::handler))[payload.m_destinationStepIndex].end(),
						sourceStepIter->second.begin(),
						sourceStepIter->second.end()
					);
				}
			}

			return true;
		}

		//TODO: perhaps report error at this point
		return false;
	}

	template <class T>
	bool swapAction(ActionDragDropPayload payload) {
		auto sourceStepIter{ (m_nodeDerived->*(chapterNodeHelper<T>::handler)).find(payload.m_sourceStepIndex) };
		auto destinationStepIter{ (m_nodeDerived->*(chapterNodeHelper<T>::handler)).find(payload.m_destinationStepIndex) };

		// Validate swap
		// The payload cannot be a pick one from a vector action type
		// The source and destination must have the the action
		if (
			payload.m_pickOne ||
			sourceStepIter == destinationStepIter ||
			destinationStepIter == (m_nodeDerived->*(chapterNodeHelper<T>::handler)).end() ||
			sourceStepIter == (m_nodeDerived->*(chapterNodeHelper<T>::handler)).end()
		   )
		{
			return false;
		}

		// Perform swap
		auto tempSourceActionCopy{ sourceStepIter->second };
		sourceStepIter->second = destinationStepIter->second;
		destinationStepIter->second = tempSourceActionCopy;
	
		return true;
	}

	template <class T>
	bool deleteAction(ActionDragDropPayload payload) {
		auto sourceStepIter{ (m_nodeDerived->*(chapterNodeHelper<T>::handler)).find(payload.m_sourceStepIndex) };

		if (sourceStepIter == (m_nodeDerived->*(chapterNodeHelper<T>::handler)).end()) {
			return false;
		}

		if (payload.m_pickOne) {
			// Erase the entry within the vector for the step
			auto pickOneIter{ sourceStepIter->second.begin() + (payload.m_souceStepPickOneIndex) };
				
			if (pickOneIter >= sourceStepIter->second.end()) {
				return false;
			}

			sourceStepIter->second.erase(pickOneIter);
		}
		else {
			// Erase the entire step for this type
			(m_nodeDerived->*(chapterNodeHelper<T>::handler)).erase(sourceStepIter);
		}

		return true;
	}

	template <class T>
	bool containsStep(index stepIndex) {
		auto iter{ (m_nodeDerived->*(chapterNodeHelper<T>::handler)).find(stepIndex) };

		if (iter == (m_nodeDerived->*(chapterNodeHelper<T>::handler)).end()) {
			return false;
		}

		return true;
	}

	template <class T>
	std::vector<T>* getStepActions(int stepIndex) {
		auto iter{ (m_nodeDerived->*(chapterNodeHelper<T>::handler)).find(stepIndex) };

		if (iter == (m_nodeDerived->*(chapterNodeHelper<T>::handler)).end()) {
			return nullptr;
		}

		return &(iter->second);
	}

	/*template <class T>
	std::unordered_map<int, T>& getStep(int stepIndex) {
		return (m_nodeDerived->*(chapterNodeHelper<T>::handler))[stepIndex];
	}

	template <class T>
	std::unordered_map<int, std::vector<T>>& getStep(int stepIndex) {
		return (m_nodeDerived->*(chapterNodeHelper<T>::handler))[stepIndex];
	}*/


	void decrementSteps() {
		m_nodeDerived->m_totalSteps--;
	}

	void incrementSteps() {
		m_nodeDerived->m_totalSteps++;
	}

private:
	void updateStepActionsArray() {

	}

private:
	template<typename> struct chapterNodeHelper;

	template<> struct chapterNodeHelper<ActionBackgroundTexture> { static constexpr auto handler = &ChapterNode::m_backgroundSteps; };

	template<> struct chapterNodeHelper<ActionSpriteProperty> { static constexpr auto handler = &ChapterNode::m_spriteTextureSteps; };
	template<> struct chapterNodeHelper<ActionSpriteAnimationGeneric> { static constexpr auto handler = &ChapterNode::m_spriteGenericAnimationSteps; };

	template<> struct chapterNodeHelper<ActionTextRender> { static constexpr auto handler = &ChapterNode::m_textRenderSteps; };
	template<> struct chapterNodeHelper<ActionTextLine> { static constexpr auto handler = &ChapterNode::m_textLineSteps; };
	template<> struct chapterNodeHelper<ActionTextOverrideSpeaker> { static constexpr auto handler = &ChapterNode::m_textOverrideSpeakerSteps; };
	template<> struct chapterNodeHelper<ActionTextOverrideColor> { static constexpr auto handler = &ChapterNode::m_textOverrideColorSteps; };

	template<> struct chapterNodeHelper<ActionRelationModify> { static constexpr auto handler = &ChapterNode::m_relationshipModifySteps; };
	template<> struct chapterNodeHelper<ActionRelationSetNextNode> { static constexpr auto handler = &ChapterNode::m_relationshipChooseNode; };
	template<> struct chapterNodeHelper<ActionRelationSetNextChapter> { static constexpr auto handler = &ChapterNode::m_relationshipChooseChapter; };

	template<> struct chapterNodeHelper<ActionSetNextChapter> { static constexpr auto handler = &ChapterNode::m_setNextChapter; };

	template<> struct chapterNodeHelper<ActionChoice> { static constexpr auto handler = &ChapterNode::m_choiceTextOptions; };
	template<> struct chapterNodeHelper<ActionChoiceSetNextNode> { static constexpr auto handler = &ChapterNode::m_choiceSetNextNode; };
	template<> struct chapterNodeHelper<ActionChoiceModifyRelation> { static constexpr auto handler = &ChapterNode::m_choiceRelationModifications; };
	template<> struct chapterNodeHelper<ActionChoiceSetNextChapter> { static constexpr auto handler = &ChapterNode::m_choiceSetNextChapter; };


};

#endif // VN_CHAPTER_NODE_BUILDER_H