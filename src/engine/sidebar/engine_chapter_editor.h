#ifndef ENGINE_CHAPTER_CONFIG_EDITOR_H
#define ENGINE_CHAPTER_CONFIG_EDITOR_H

#include "state_subject.h"

class VnEngineChapterEditor {
private:
	static StateSubject* m_stateSubject;

public:
	VnEngineChapterEditor(StateSubject* stateSubject)
	{
		m_stateSubject = stateSubject;
	}

	~VnEngineChapterEditor() {}


public:
	void draw() {

	}

};


#endif // ENGINE_CHAPTER_CONFIG_EDITOR_H