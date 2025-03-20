#ifndef VN_ENGINE_CHAPTER_MANAGER_H
#define VN_ENGINE_CHAPTER_MANAGER_H

#include "id.h"
#include "chapter.h"

#include <memory>
#include <unordered_map>

class EngineChapterManager {
public:
	using ChapterMap = std::unordered_map<id, std::unique_ptr<Chapter>>;

private:
	ChapterMap m_chapters{};

public:
	EngineChapterManager() {}
	~EngineChapterManager() {}

	Chapter* get(id chapterId) {
		// TODO: Load from model file if not loaded

		// TODO: this is temp for testing
		auto iter{ m_chapters.find(chapterId) };

		if (iter != m_chapters.end()) {
			return iter->second.get();
		}

		return nullptr;
	}

	const ChapterMap& getChapters() {
		return m_chapters;
	}

	void add(Chapter* newChapter) {
		if (newChapter == nullptr) {
			return;
		}

		m_chapters[newChapter->getId()] = std::unique_ptr<Chapter>{ newChapter };
	}

	// load all chapter files
	void load() {}

	// save/overwrite all chapter files
	void save() {}

	// save/overwrite specific chapter?
	void save(id chapterId) {}
};


#endif // VN_ENGINE_CHAPTER_MANAGER_H