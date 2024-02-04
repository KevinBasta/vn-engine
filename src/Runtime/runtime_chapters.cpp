
#include "node.h"
#include "node_children.h"
#include "graph.h"
#include "chapter.h"

#include <vector>

std::vector<std::unique_ptr<Chapter>> g_chapters{};

void createChapterOne() {
	std::unique_ptr<Chapter> chapterOne{ std::make_unique<Chapter>() };

	Graph& chapterOneGraph{ chapterOne.get()->getGraph() };
	
	Node* chapterOneHead{ new Node("the head node!") };
	Node* chapterOneChild1{ new Node("level 1 child node one!") };
	Node* chapterOneChild2{ new Node("level 1 child node two!") };
	Node* chapterOneChild3{ new Node("level 1 child node three!") };

	Node* chapterOneChild4{ new Node("level 2 child node 1!") };
	
	Node* chapterOneChild5{ new Node("level 3 child node 1!") };

	chapterOneHead->addChild(chapterOneChild1);
	chapterOneHead->addChild(chapterOneChild2);
	chapterOneHead->addChild(chapterOneChild3);

	chapterOneChild1->addChild(chapterOneChild4);
	chapterOneChild4->addChild(chapterOneChild5);
	// chapterOneHead->addChild(chapterOneHead); unhandled case yet

	chapterOneGraph.setHeadNode(chapterOneHead);

	std::cout << chapterOneGraph << std::endl;
}