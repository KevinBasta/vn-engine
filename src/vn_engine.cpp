// vn_engine.cpp : Defines the entry point for the application.
//

#include "vn_engine.h"

#include "id.h"
#include "timer.h"
#include "window.h"

#include "subjects_orchestrator.h"
#include "model_subject.h"
#include "state_subject.h"

#include "game_loop.h"
#include "game_observer.h"
#include "engine_observer.h"

#include <iostream>
#include <string>
#include <filesystem>
#include <memory>

#ifdef _WIN32
#include <crtdbg.h>
#endif

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
	std::cout << "checking normal operation" << std::endl;

	std::cout << VN_BASE_PATH << std::endl;

	// load opengl
	VnWindow window{};
	window.load();
	
	// contains state of what chapter/node on and character relations and items


	// contatins current game state
	StateSubject stateSubject{};

	SubjectsOrchestrator subjectsOrchestrator{&stateSubject};
	subjectsOrchestrator.newGame();

	// the vn game
	GameObserver game{ &window, &stateSubject };
	
	stateSubject.attatch(static_cast<Observer*>(&game));

	game.run();

	
	//subject.forwardProgress(&stateSubject);
}

void idCheck() {
	std::cout << IdGenerator<Character>::getId() << std::endl;
	std::cout << IdGenerator<Character>::getId() << std::endl;
	std::cout << IdGenerator<Character>::getId() << std::endl;

	std::cout << IdGenerator<Node>::getId() << std::endl;
	std::cout << IdGenerator<Node>::getId() << std::endl;
	std::cout << IdGenerator<Node>::getId() << std::endl;
	
	std::cout << IdGenerator<Character>::getId() << std::endl;
}

int main()
{
	std::cout << "Hello CMake." << std::endl;
	std::cout << "before " << std::endl;

	//idCheck();
	memCheck();
	
	std::cout << "End of program" << std::endl;

#ifdef _WIN32
	std::cout << "RUNNING MEMORY LEAK CHECK" << std::endl;
	// Check for memory leaks.
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//bool test = _CrtDumpMemoryLeaks();
	//std::cout << ((test == 1) ? "MEMORY LEAK PRESENT" : "NO MEMORY LEAK DETECTED") << std::endl;
#endif


	return 0;
}
