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
	StateSubject stateSubject{};

	// contains state of what chapter/node on and character relations and items
	ModelSubject modelSubject{};
	modelSubject.initCharacters();
	modelSubject.createChapterOne();
	modelSubject.attatchStateSubject(&stateSubject);

	// the vn game
	GameObserver game{ window.get()->getWindow(), &stateSubject };
	game.run();
	
	//subject.forwardProgress(&stateSubject);
}

int main()
{
	std::cout << "Hello CMake." << std::endl;
	std::cout << "before " << std::endl;

	memCheck();
	
	std::cout << "End of program" << std::endl;


	// Check for memory leaks. Does not account for global variables or static variables.
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
	bool test = _CrtDumpMemoryLeaks();
	std::cout << ((test == 1) ? "MEMORY LEAK PRESENT" : "NO MEMORY LEAK DETECTED") << std::endl;

	return 0;
}
