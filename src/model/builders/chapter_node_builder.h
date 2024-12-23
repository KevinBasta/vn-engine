#ifndef VN_CHAPTER_NODE_BUILDER_H
#define VN_CHAPTER_NODE_BUILDER_H

#include "chapter_node.h"
#include "chapter_node_types.h"

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
	void removeStep(int stepIndex) {
		(m_nodeDerived->*(chapterNodeHelper<T>::handler)).erase(stepIndex);
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



	//template <class T>
	//T* getStepAction(int stepIndex) {
	//	return &((m_nodeDerived->*(chapterNodeHelper<T>::handler))[stepIndex]);
	//}

	template <class T>
	std::vector<T>& getStepActions(int stepIndex) {
		// TODO: ERR, RETURN IS CREATING ENTRY IN MAP AND RETURNING EMPTY VECTOR
		
		if ((m_nodeDerived->*(chapterNodeHelper<T>::handler)).find(stepIndex) == (m_nodeDerived->*(chapterNodeHelper<T>::handler)).end()) {
			//throw someObj
		}

		return (m_nodeDerived->*(chapterNodeHelper<T>::handler))[stepIndex];
	}

	/*template <class T>
	std::unordered_map<int, T>& getStep(int stepIndex) {
		return (m_nodeDerived->*(chapterNodeHelper<T>::handler))[stepIndex];
	}

	template <class T>
	std::unordered_map<int, std::vector<T>>& getStep(int stepIndex) {
		return (m_nodeDerived->*(chapterNodeHelper<T>::handler))[stepIndex];
	}*/

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