

#include "id.h"

#include "model_subject.h"
#include "chapter_iterator.h"

std::unique_ptr<ModelSubject> ModelSubject::m_instance{ nullptr };

ChapterIterator ModelSubject::iter() {
	ModelSubject* model{ checkInstance() };

	// TODO: provide the id of the first chapter according to model load/save files
	return ChapterIterator( 1 );
}

ChapterIterator ModelSubject::iter(id chapterId) {
	ModelSubject* model{ checkInstance() };

	return ChapterIterator(chapterId);
}
