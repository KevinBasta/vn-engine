

#include <list>
#include <string>

/*
 * A story node, should have side effects on characters?
 * Can contain a normal text, characters, positions, player talking, etc..
 * Can also contain an opengl game? or maybe that should be a separate note type
 * Maybe node subtypes
 */
class Node {
private:
	std::list<Node&> children{};
	// pre actions (transitions, animations, etc..)
	// body actions (text, animations, etc..)
	// post actions (choises, bond mutations, transitions, animations, etc...)

public:
	/*Node& selectNode(int n) {
		if (n > children.size()) {
			// raise execption
		}

	}*/
	
	
	virtual std::string_view displayNode() {
		
	}

	void action() {
		// pre
		// ret = theaction();
		// do post actions based on ret
	}
	


};

class ChapterNode: Node {

};

class OpenGLNode : Node {

};