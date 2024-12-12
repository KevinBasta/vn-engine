#ifndef MODEL_INTERFACE_COMMON_H
#define MODEL_INTERFACE_COMMON_H

#include "model_subject.h"

#include "chapter_iterator.h"

class ModelCommonInterface {
public:
	static id getFirstChapterId() {
		return 1;
	}

	static const Chapter* getHeadChapter() {
		ModelSubject* model{ ModelSubject::validateInstance() };

		return model->m_modelChapters.getHeadChapter();
	}

	static Chapter* getChapterById(id chapterId) {
		ModelSubject* model{ ModelSubject::validateInstance() };

		return model->m_modelChapters.getChapterById(chapterId);
	}

	static Node* getNodeById(id nodeId) {
		ModelSubject* model{ ModelSubject::validateInstance() };

		return model->m_modelChapters.getNodeById(nodeId);
	}

	static ChapterIterator iter() {
		ModelSubject* model{ ModelSubject::validateInstance() };

		// TODO: provide the id of the first chapter according to model load/save files
		return ChapterIterator(1);
	}

	static ChapterIterator iter(id chapterId) {
		ModelSubject* model{ ModelSubject::validateInstance() };

		return ChapterIterator(chapterId);
	}


};


#endif // MODEL_INTERFACE_COMMON_H