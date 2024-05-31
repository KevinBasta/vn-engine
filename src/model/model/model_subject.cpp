
#include "model_subject.h"
#include "chapter_iterator.h"

ChapterIterator ModelSubject::iter(int chapterIndex) {
	 return ChapterIterator(this, chapterIndex);
}
