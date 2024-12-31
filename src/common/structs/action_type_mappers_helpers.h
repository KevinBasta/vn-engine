
#ifndef VN_ACTION_TYPE_MAPPERS_HELPERS_H
#define VN_ACTION_TYPE_MAPPERS_HELPERS_H

#include "state_subject.h"
#include "chapter_node.h"
#include "chapter_node_types.h"
#include "action_type_mappers.h"

#include <string>

enum ActionAmount;

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


// For engine_node_editor.h and other
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



// For chapter_node.cpp
//template<typename> struct ActionToStateHandler;

// For chapter_node_builder.h
//template<typename> struct ActionToNodeVariable;

//template<> struct ActionToStateHandler<ActionBackgroundTexture> { static constexpr auto handler = &StateSubject::m_background; };
//
//template<> struct ActionToStateHandler<ActionSpriteProperty> { static constexpr auto handler = &StateSubject::m_sprites; };
//template<> struct ActionToStateHandler<ActionSpriteAnimationGeneric> { static constexpr auto handler = &StateSubject::m_sprites; };
//
//template<> struct ActionToStateHandler<ActionTextRender> { static constexpr auto handler = &StateSubject::m_dialogue; };
//template<> struct ActionToStateHandler<ActionTextLine> { static constexpr auto handler = &StateSubject::m_dialogue; };
//template<> struct ActionToStateHandler<ActionTextOverrideSpeaker> { static constexpr auto handler = &StateSubject::m_dialogue; };
//template<> struct ActionToStateHandler<ActionTextOverrideColor> { static constexpr auto handler = &StateSubject::m_dialogue; };
//
//template<> struct ActionToStateHandler<ActionRelationModify> { static constexpr auto handler = &StateSubject::m_relations; };
//template<> struct ActionToStateHandler<ActionRelationSetNextNode> { static constexpr auto handler = &StateSubject::m_relations; };
//template<> struct ActionToStateHandler<ActionRelationSetNextChapter> { static constexpr auto handler = &StateSubject::m_relations; };
//
//template<> struct ActionToStateHandler<ActionSetNextChapter> { static constexpr auto handler = &StateSubject::m_nextChapter; };
//
//template<> struct ActionToStateHandler<ActionChoice> { static constexpr auto handler = &StateSubject::m_choices; };
//template<> struct ActionToStateHandler<ActionChoiceSetNextNode> { static constexpr auto handler = &StateSubject::m_choices; };
//template<> struct ActionToStateHandler<ActionChoiceModifyRelation> { static constexpr auto handler = &StateSubject::m_choices; };
//template<> struct ActionToStateHandler<ActionChoiceSetNextChapter> { static constexpr auto handler = &StateSubject::m_choices; };


//template<> struct ActionToNodeVariable<ActionBackgroundTexture> { static constexpr auto handler = &ChapterNode::m_backgroundSteps; };
//
//template<> struct ActionToNodeVariable<ActionSpriteProperty> { static constexpr auto handler = &ChapterNode::m_spriteTextureSteps; };
//template<> struct ActionToNodeVariable<ActionSpriteAnimationGeneric> { static constexpr auto handler = &ChapterNode::m_spriteGenericAnimationSteps; };
//
//template<> struct ActionToNodeVariable<ActionTextRender> { static constexpr auto handler = &ChapterNode::m_textRenderSteps; };
//template<> struct ActionToNodeVariable<ActionTextLine> { static constexpr auto handler = &ChapterNode::m_textLineSteps; };
//template<> struct ActionToNodeVariable<ActionTextOverrideSpeaker> { static constexpr auto handler = &ChapterNode::m_textOverrideSpeakerSteps; };
//template<> struct ActionToNodeVariable<ActionTextOverrideColor> { static constexpr auto handler = &ChapterNode::m_textOverrideColorSteps; };
//
//template<> struct ActionToNodeVariable<ActionRelationModify> { static constexpr auto handler = &ChapterNode::m_relationshipModifySteps; };
//template<> struct ActionToNodeVariable<ActionRelationSetNextNode> { static constexpr auto handler = &ChapterNode::m_relationshipChooseNode; };
//template<> struct ActionToNodeVariable<ActionRelationSetNextChapter> { static constexpr auto handler = &ChapterNode::m_relationshipChooseChapter; };
//
//template<> struct ActionToNodeVariable<ActionSetNextChapter> { static constexpr auto handler = &ChapterNode::m_setNextChapter; };
//
//template<> struct ActionToNodeVariable<ActionChoice> { static constexpr auto handler = &ChapterNode::m_choiceTextOptions; };
//template<> struct ActionToNodeVariable<ActionChoiceSetNextNode> { static constexpr auto handler = &ChapterNode::m_choiceSetNextNode; };
//template<> struct ActionToNodeVariable<ActionChoiceModifyRelation> { static constexpr auto handler = &ChapterNode::m_choiceRelationModifications; };
//template<> struct ActionToNodeVariable<ActionChoiceSetNextChapter> { static constexpr auto handler = &ChapterNode::m_choiceSetNextChapter; };

#endif // VN_ACTION_TYPE_MAPPERS_HELPERS_H
