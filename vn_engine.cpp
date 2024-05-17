// vn_engine.cpp : Defines the entry point for the application.
//

#include "vn_engine.h"
#include "timer.h"

#include <iostream>
#include <string>
#include <filesystem>

#include "frame.h"
#include "model_subject.h"
#include "game_observer.h"

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
	
	ModelSubject subject{};
	
	GameObserver game{ &subject }; // separate the opengl initalizeation to decouple this dependancy of Texture2D
	
	subject.initCharacters();
	subject.createChapterOne();
	
	game.run();
	
	std::cout << "End of program" << std::endl;
	return 0;
}
