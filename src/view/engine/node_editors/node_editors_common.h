
#ifndef VN_NODE_EDITORS_COMMON_H
#define VN_NODE_EDITORS_COMMON_H

#include "id.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui_node_editor.h"

namespace ed = ax::NodeEditor;

namespace std
{
	template<> struct hash<ed::PinId>
	{
		std::size_t operator()(const ed::PinId& s) const noexcept
		{
			return static_cast<std::size_t>(s.Get());
		}
	};

	template<> struct hash<ed::NodeId>
	{
		std::size_t operator()(const ed::NodeId& s) const noexcept
		{
			return static_cast<std::size_t>(s.Get());
		}
	};
}

struct NodeIdHasher {
	size_t operator()(const ed::NodeId& key) const {
		return std::hash<ed::NodeId>()(key);
	}
};

struct PinIdHasher {
	size_t operator()(const ed::PinId& key) const {
		return std::hash<ed::PinId>()(key);
	}
};

struct NodeLinkKey {
	id m_firstNode{};
	id m_secondNode{};

	friend bool operator==(const NodeLinkKey& keyOne, const NodeLinkKey& keyTwo);
};

struct NodeLinkKeyHasher {
	size_t operator()(const NodeLinkKey& key) const {
		return std::hash<id>()(key.m_firstNode) ^
			(std::hash<id>()(key.m_secondNode) << 1);
	}
};

struct NodeLinkData {
	ed::LinkId m_id{};
	ed::PinId m_inId{};
	ed::PinId m_outId{};
};

#endif // VN_NODE_EDITORS_COMMON_H