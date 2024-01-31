
#include "relation.h"
#include "relation_types.h"
#include "runtime_characters.h"

void initRelations() {
	RelationTypes::addRelationType("friendship");
	RelationTypes::addRelationType("respect");
	RelationTypes::addRelationType("hatred");
	RelationTypes::print();

	Character* garu = g_characters[0].get();
	Relations& garuRelations = (*garu).getRelationsObject();

	Character* brz = g_characters[1].get();
	Relations& brzRelations = (*brz).getRelationsObject();

	int friendshipId = 0; RelationTypes::getRelationId("friendship", friendshipId);
	int respectId = 0;    RelationTypes::getRelationId("respect", respectId);
	int hatredId = 0;     RelationTypes::getRelationId("hatred", hatredId);

	int garuId = garu->getId();
	int brzId = brz->getId();

	std::cout << garuId << brzId << friendshipId << respectId << hatredId << std::endl;

	garuRelations.addCharacterRelation(brzId, friendshipId, 1);
	std::cout << garuRelations;
	garuRelations.addCharacterRelation(brzId, hatredId, 20);
	std::cout << garuRelations;
	garuRelations.addCharacterRelation(brzId, hatredId, 2);
	std::cout << garuRelations;
	garuRelations.addCharacterRelation(brzId, hatredId, -10);
	std::cout << garuRelations;

	brzRelations.addCharacterRelation(garuId, respectId, 1);
}