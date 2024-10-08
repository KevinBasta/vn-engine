
#ifndef CHAPTER_H
#define CHAPTER_H

#include "id.h"
#include "linkable.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <set>
#include <string>
#include <memory>

class ChapterBuilder;

class Chapter : public Linkable {
private:
	friend class ChapterBuilder;

	std::wstring m_name{};
	
	// First node in the graph
	id m_headNodeId{};

public:
	Chapter() {}
	std::wstring getName() const { return m_name; }
	id getHeadNodeId() const { return m_headNodeId; }
};

#endif // CHAPTER_H
