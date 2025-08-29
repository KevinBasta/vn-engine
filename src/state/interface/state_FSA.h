#ifndef VN_STATE_FSA_H
#define VN_STATE_FSA_H

#include <iostream>

enum class VNState {
	MAIN_MENU,
	SAVES_MENU_LOAD,
	SAVES_MENU_SAVE,
	OPTIONS_MENU,
	IN_GAME,
	IN_GAME_WITH_SIDE_BAR,
	QUIT
};

class VNFSA {
public:
	static inline VNState gameState{ VNState::MAIN_MENU };

	static bool validateTransition(VNState newState) {
		// TODO
		return true;
	}

	static void printCurrent() {
		std::cout << int(gameState) << std::endl;
	}

	static bool transition(VNState newState) {
		//validateTransition(newState);
		gameState = newState;

		return true;
	}

	static bool inMainMenu()			{ return VNFSA::gameState == VNState::MAIN_MENU; }
	static bool inSavesToLoad()			{ return VNFSA::gameState == VNState::SAVES_MENU_LOAD; }
	static bool inSavesToSave()			{ return VNFSA::gameState == VNState::SAVES_MENU_SAVE; }
	static bool inSavesMenu()			{ return inSavesToLoad() || inSavesToSave(); }
	static bool inOptionsMenu()			{ return VNFSA::gameState == VNState::OPTIONS_MENU; }
	static bool optionsSidebarOpen()	{ return VNFSA::gameState == VNState::IN_GAME_WITH_SIDE_BAR; }
	static bool inGame()				{ return VNFSA::gameState == VNState::IN_GAME || optionsSidebarOpen(); }
	static bool inQuitState()			{ return VNFSA::gameState == VNState::QUIT; }
};

#endif // VN_STATE_FSA_H