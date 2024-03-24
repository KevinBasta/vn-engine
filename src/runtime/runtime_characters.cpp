
#include "character.h"
#include <vector>
#include <memory>

std::vector<std::unique_ptr<Character>> g_characters{};

void initCharacters() {
	std::unique_ptr<Character> garu{ std::make_unique<Character>("garu") };
	std::unique_ptr<Character> brz{ std::make_unique<Character>("brazazaza") };

	garu.get()->addTexture("C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\assets\\garu_outline.png");

	g_characters.push_back(std::move(garu));
	g_characters.push_back(std::move(brz));
}