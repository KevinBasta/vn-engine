
#include "character.h"
#include <vector>

std::vector<Character> g_characters{};

void initCharacters() {

	// these are being pushed as copies for now
	// multiple ways to make this better using
	// move semantics

	Character garu{ "garu" };	
	Character brz{ "brazazaza" };

	g_characters.push_back(garu);
	g_characters.push_back(brz);
}