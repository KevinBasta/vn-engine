#ifndef VN_ENGINE_HELPERS_H
#define VN_ENGINE_HELPERS_H

#include "node_types.h"

#include <locale>
#include <codecvt>
#include <string>
#include <algorithm>

static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> myconv;

static std::string addIdFromPtr(std::string name, void* ptr) { return name + "##" + std::to_string((unsigned long long)(void**)ptr); }
static std::string addIdFromPtr(std::string name, const void* ptr) { return name + "##" + std::to_string((unsigned long long)(void**)ptr); }
static std::string addIdFromPtr(std::wstring name, void* ptr) {
	return myconv.to_bytes(name) + std::to_string((unsigned long long)(void**)ptr);
}

static std::string toString(enum class SpriteProperty property) {
	switch (property)
	{
	case SpriteProperty::XPOS:
		return "XPOS";
	case SpriteProperty::YPOS:
		return "YPOS";
	case SpriteProperty::ZPOS:
		return "ZPOS";
	case SpriteProperty::SCALE:
		return "SCALE";
	case SpriteProperty::ROTATION:
		return "ROTATION";
	case SpriteProperty::OPACITY:
		return "OPACITY";
	default:
		break;
	}

	return "NONE";
}

static std::string toString(enum class RelationModification modification) {
	switch (modification)
	{
	case RelationModification::SET:
		return "SET";
	case RelationModification::ADD:
		return "ADD";
	case RelationModification::SUBTRACT:
		return "SUBTRACT";
	case RelationModification::MULTIPLY:
		return "MULTIPLY";
	case RelationModification::DIVIDE:
		return "DIVIDE";
	default:
		break;
	}

	return "NONE";
}

static std::string toString(enum class ChoiceStyle style) {
	switch (style)
	{
	case ChoiceStyle::LIST_MID_SCREEN:
		return "LIST MID SCREEN";
	case ChoiceStyle::LIST_TEXT_AREA:
		return "LIST TEXT AREA";
	default:
		break;
	}

	return "NONE";
}


#endif // VN_ENGINE_HELPERS_H