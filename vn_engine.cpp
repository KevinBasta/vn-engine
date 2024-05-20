// vn_engine.cpp : Defines the entry point for the application.
//

#include "vn_engine.h"

#include "timer.h"

#include "frame.h"

#include "window.h"
#include "model_subject.h"
#include "state_subject.h"
#include "game_observer.h"

#include <iostream>
#include <string>
#include <filesystem>

void testCore() {

	//initRelations();

	/*for (auto it = std::begin(g_characters); it != std::end(g_characters); ++it) {
		std::cout << **it;
	}*/

	// initRelations();

	/*for (int i = 0; i < g_characters.size(); i++) {
		Character* temp = g_characters[i].get();
		std::cout << *temp << std::endl;
	}*/

}


int main()
{
	std::cout << "Hello CMake." << std::endl;
	
	VnWindow window{};
	window.load();

	// contains state of what chapter/node on and character relations and items
	ModelSubject subject{};
	subject.initCharacters();
	subject.createChapterOne();
	
	// contatins all other state
	StateSubject stateSubject{};

	GameObserver game{ window.getWindow() ,& subject }; // separate the opengl initalizeation to decouple this dependancy of Texture2D
	
	

	subject.forwardProgress(&stateSubject);

	game.run();
	
	std::cout << "End of program" << std::endl;
	return 0;
}
