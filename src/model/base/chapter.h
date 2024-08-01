
#ifndef CHAPTER_H
#define CHAPTER_H

#include "id.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <set>
#include <string>
#include <memory>

class ChapterBuilder;

class Chapter {
private:
	friend class ChapterBuilder;

	std::wstring m_name{};
	id m_headNodeId{};
	id m_id{};

	// For the purposes of traversal 
	std::set<id> m_parents{};
	std::set<id> m_children{};

public:
	Chapter() {}

	id getId() const { return m_id; }
	id getHeadNodeId() const { return m_headNodeId; }
};

#endif // CHAPTER_H
