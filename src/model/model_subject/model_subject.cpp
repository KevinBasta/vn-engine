
#include "model_subject.h"
#include "chapter_iterator.h"

std::unique_ptr<ModelSubject> ModelSubject::m_instance{ nullptr };

ChapterIterator ModelSubject::iter(int chapterIndex) {
	checkInstance();

	ModelSubject* model{ m_instance.get() };

	return ChapterIterator(model, chapterIndex);
}
