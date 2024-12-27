
#ifndef VN_ACTION_TYPE_MAPPERS_H
#define VN_ACTION_TYPE_MAPPERS_H

#include "chapter_node_types.h"

#include "engine_node_action_fields.h"

// For chapter_node.cpp
//template<typename> struct ActionToStateHandler;

// For chapter_node_builder.h
//template<typename> struct ActionToNodeVariable;

// For engine_node_editor.h
template<typename> struct ActionToActionName;
template<> struct ActionToActionName<ActionBackgroundTexture>		{ static constexpr const char* name = "ActionBackgroundTexture"; };

template<> struct ActionToActionName<ActionSpriteProperty>			{ static constexpr const char* name = "ActionSpriteProperty"; };
template<> struct ActionToActionName<ActionSpriteAnimationGeneric>	{ static constexpr const char* name = "ActionSpriteAnimationGeneric"; };

template<> struct ActionToActionName<ActionTextRender>				{ static constexpr const char* name = "ActionTextRender"; };
template<> struct ActionToActionName<ActionTextLine>				{ static constexpr const char* name = "ActionTextLine"; };
template<> struct ActionToActionName<ActionTextOverrideSpeaker>		{ static constexpr const char* name = "ActionTextOverrideSpeaker"; };
template<> struct ActionToActionName<ActionTextOverrideColor>		{ static constexpr const char* name = "ActionTextOverrideColor"; };

template<> struct ActionToActionName<ActionRelationModify>			{ static constexpr const char* name = "ActionRelationModify"; };
template<> struct ActionToActionName<ActionRelationSetNextNode>		{ static constexpr const char* name = "ActionRelationSetNextNode"; };
template<> struct ActionToActionName<ActionRelationSetNextChapter>	{ static constexpr const char* name = "ActionRelationSetNextChapter"; };

template<> struct ActionToActionName<ActionSetNextChapter>			{ static constexpr const char* name = "ActionSetNextChapter"; };

template<> struct ActionToActionName<ActionChoice>					{ static constexpr const char* name = "ActionChoice"; };
template<> struct ActionToActionName<ActionChoiceSetNextNode>		{ static constexpr const char* name = "ActionChoiceSetNextNode"; };
template<> struct ActionToActionName<ActionChoiceModifyRelation>	{ static constexpr const char* name = "ActionChoiceModifyRelation"; };
template<> struct ActionToActionName<ActionChoiceSetNextChapter>	{ static constexpr const char* name = "ActionChoiceSetNextChapter"; };


enum ActionAmount {
	SINGLE,
	VECTOR
};

template<typename> struct ActionToType;
template<> struct ActionToType<ActionBackgroundTexture>			{ static constexpr ActionAmount type = ActionAmount::SINGLE; };

template<> struct ActionToType<ActionSpriteProperty>			{ static constexpr ActionAmount type = ActionAmount::VECTOR; };
template<> struct ActionToType<ActionSpriteAnimationGeneric>	{ static constexpr ActionAmount type = ActionAmount::VECTOR; };

template<> struct ActionToType<ActionTextRender>				{ static constexpr ActionAmount type = ActionAmount::SINGLE; };
template<> struct ActionToType<ActionTextLine>					{ static constexpr ActionAmount type = ActionAmount::SINGLE; };
template<> struct ActionToType<ActionTextOverrideSpeaker>		{ static constexpr ActionAmount type = ActionAmount::SINGLE; };
template<> struct ActionToType<ActionTextOverrideColor>			{ static constexpr ActionAmount type = ActionAmount::SINGLE; };

template<> struct ActionToType<ActionRelationModify>			{ static constexpr ActionAmount type = ActionAmount::SINGLE; }; // TODO: THIS SHOULD BE VECTOR
template<> struct ActionToType<ActionRelationSetNextNode>		{ static constexpr ActionAmount type = ActionAmount::VECTOR; };
template<> struct ActionToType<ActionRelationSetNextChapter>	{ static constexpr ActionAmount type = ActionAmount::VECTOR; };

template<> struct ActionToType<ActionSetNextChapter>			{ static constexpr ActionAmount type = ActionAmount::SINGLE; };

template<> struct ActionToType<ActionChoice>					{ static constexpr ActionAmount type = ActionAmount::SINGLE; };
template<> struct ActionToType<ActionChoiceSetNextNode>			{ static constexpr ActionAmount type = ActionAmount::SINGLE; };
template<> struct ActionToType<ActionChoiceModifyRelation>		{ static constexpr ActionAmount type = ActionAmount::SINGLE; };
template<> struct ActionToType<ActionChoiceSetNextChapter>		{ static constexpr ActionAmount type = ActionAmount::SINGLE; };

// Helper to avoid having to specify more data with the specific types T
// TODO: this can be applicable to so much more, especially the model/state
// and the helper structs they use to get pointers to functions and variables
struct ActionHelper {
public:
	template <class T>
	ActionHelper(std::in_place_type_t<T>)
		: getType([]() { return ActionToType<T>::type; }),
		getName([]() { return ActionToActionName<T>::name; }),
		drawNew([]() { return ActionField<T>::drawField(); }),
		drawExisting([](ChapterNode* node, int index) {
			return ActionField<T>::drawField(node, index);
		})
	{
	}

	std::function<ActionAmount()> getType;
	std::function<bool()> drawNew;
	std::function<bool(ChapterNode* node, int index)> drawExisting;
	std::function<const char* ()> getName;
};


#endif // VN_ACTION_TYPE_MAPPERS_H
