
#include "node.h"
#include "node_children.h"
#include "graph.h"
#include "chapter.h"

#include <vector>

//std::vector<std::unique_ptr<Chapter>> g_chapters{};
//
//void createChapterOne() {
//	Chapter* chapterOne = new Chapter{};
//
//	Graph& chapterOneGraph{ chapterOne->getGraph() };
//	
//	Node* chapterOneHead{ new Node("the head node!") };
//	Node* chapterOneChild1{ new Node("level 1 child node 1!") };
//	Node* chapterOneChild2{ new Node("level 1 child node 2!") };
//	Node* chapterOneChild3{ new Node("level 1 child node 3!") };
//
//	Node* chapterOneChild4{ new Node("level 2 child node 1!") };
//	
//	Node* chapterOneChild5{ new Node("level 2 child node 2!") };
//
//	chapterOneHead->addChild(chapterOneChild1);
//	chapterOneHead->addChild(chapterOneChild2);
//	chapterOneHead->addChild(chapterOneChild3);
//
//	chapterOneChild1->addChild(chapterOneChild4);
//	chapterOneChild1->addChild(chapterOneChild2);
//	chapterOneChild2->addChild(chapterOneChild5);
//	
//	//chapterOneChild4->addChild(chapterOneChild5);
//	// chapterOneHead->addChild(chapterOneHead); unhandled case yet
//
//	chapterOneGraph.setHeadNode(chapterOneHead);
//
//	g_chapters.push_back(std::unique_ptr<Chapter>(chapterOne));
//	//std::cout << chapterOneGraph << std::endl;
//}