
#ifndef MODEL_ENGINE_INTERFACE_H
#define MODEL_ENGINE_INTERFACE_H

#include "model_subject.h"

class ModelEngineInterface {
	// Common interface calls the ModelSubject implementation
	static id getFirstChapterId()					{ return ModelSubject::getFirstChapterId(); }
	static const Chapter* getHeadChapter()			{ return ModelSubject::getHeadChapter(); }
	static Chapter* getChapterById(id chapterId)	{ return ModelSubject::getChapterById(chapterId); }
	static Node* getNodeById(id nodeId)				{ return ModelSubject::getNodeById(nodeId); }

};

#endif // MODEL_ENGINE_INTERFACE_H