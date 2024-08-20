#ifndef VN_STATE_NEXT_CHAPTER_H
#define VN_STATE_NEXT_CHAPTER_H

#include "id.h"
#include "chapter_node_types.h"

#include <optional>
#include <vector>

class StateNextChapter {
private:
	// Serializable
	std::vector<id> m_chapterChoicesRecord{};

	std::optional<id> m_nextChapterId{};
	
public:
	void set(id nextChapterId) {
		m_nextChapterId.emplace(nextChapterId);
	}

	void reset() {
		m_nextChapterId.reset();
	}

	bool hasNextChapterId() { return m_nextChapterId.has_value(); }
	id getNextChapterId() { return m_nextChapterId.value(); }
	void recordChapterChildChoice(id chapterId) { m_chapterChoicesRecord.push_back(chapterId); }

public:
	void handle(ActionSetNextChapter& action) {
		std::cout << "set next chapter" << std::endl;
		set(action.m_chapterId);
	}
};


#endif // VN_STATE_NEXT_CHAPTER_H