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
#include <memory>

#include <crtdbg.h>

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

void memCheck() {
	// load opengl
	std::unique_ptr<VnWindow> window{ std::make_unique<VnWindow>() };
	window.get()->load();
	std::cout << "test" << std::endl;

	// contatins current game state
	//StateSubject stateSubject{};

	// contains state of what chapter/node on and character relations and items
	//ModelSubject modelSubject{};
	//modelSubject.initCharacters();
	//modelSubject.createChapterOne();
	//modelSubject.attatchStateSubject(&stateSubject);

	// the vn game
	//GameObserver game{ window.getWindow(), &stateSubject };
	//game.run();
}

int main()
{
	std::cout << "Hello CMake." << std::endl;
	std::cout << "before " << std::endl;

	memCheck();
	//subject.forwardProgress(&stateSubject);
	
	std::cout << "End of program" << std::endl;
	bool test = _CrtDumpMemoryLeaks();

	std::cout << test << std::endl;
	return 0;
}
