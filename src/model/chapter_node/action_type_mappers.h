
#ifndef VN_ACTION_TYPE_MAPPERS_H
#define VN_ACTION_TYPE_MAPPERS_H

#include "chapter_node_types.h"

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


enum ActionType {
	SINGLE,
	VECTOR
};

template<typename> struct ActionToType;
template<> struct ActionToType<ActionBackgroundTexture>			{ static constexpr ActionType type = ActionType::SINGLE; };

template<> struct ActionToType<ActionSpriteProperty>			{ static constexpr ActionType type = ActionType::VECTOR; };
template<> struct ActionToType<ActionSpriteAnimationGeneric>	{ static constexpr ActionType type = ActionType::VECTOR; };

template<> struct ActionToType<ActionTextRender>				{ static constexpr ActionType type = ActionType::SINGLE; };
template<> struct ActionToType<ActionTextLine>					{ static constexpr ActionType type = ActionType::SINGLE; };
template<> struct ActionToType<ActionTextOverrideSpeaker>		{ static constexpr ActionType type = ActionType::SINGLE; };
template<> struct ActionToType<ActionTextOverrideColor>			{ static constexpr ActionType type = ActionType::SINGLE; };

template<> struct ActionToType<ActionRelationModify>			{ static constexpr ActionType type = ActionType::SINGLE; }; // TODO: THIS SHOULD BE VECTOR
template<> struct ActionToType<ActionRelationSetNextNode>		{ static constexpr ActionType type = ActionType::VECTOR; };
template<> struct ActionToType<ActionRelationSetNextChapter>	{ static constexpr ActionType type = ActionType::VECTOR; };

template<> struct ActionToType<ActionSetNextChapter>			{ static constexpr ActionType type = ActionType::SINGLE; };

template<> struct ActionToType<ActionChoice>					{ static constexpr ActionType type = ActionType::SINGLE; };
template<> struct ActionToType<ActionChoiceSetNextNode>			{ static constexpr ActionType type = ActionType::SINGLE; };
template<> struct ActionToType<ActionChoiceModifyRelation>		{ static constexpr ActionType type = ActionType::SINGLE; };
template<> struct ActionToType<ActionChoiceSetNextChapter>		{ static constexpr ActionType type = ActionType::SINGLE; };



#endif // VN_ACTION_TYPE_MAPPERS_H
