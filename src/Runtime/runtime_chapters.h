
#ifndef RUNTIME_CHAPTERS_H
#define RUNTIME_CHAPTERS_H

#include "node.h"
#include "node_children.h"
#include "chapter.h"

#include <vector>
#include <memory>

extern std::vector<std::unique_ptr<Chapter>> g_chapters;

void createChapterOne();

#endif //RUNTIME_CHAPTERS_H
