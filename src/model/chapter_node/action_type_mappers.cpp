
#include "state_subject.h"
#include "chapter_node.h"
#include "chapter_node_types.h"
#include "action_type_mappers.h"

#include <string>

// TODO: Bring all the action lookup helper definitons here?
// TODO: Merge all the mappers into one mapper?

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
//



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



