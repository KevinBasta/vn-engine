
#include "relation.h"

void initRelations() {
	RelationTypes::addRelationType("friendship");
	RelationTypes::addRelationType("respect");
	RelationTypes::addRelationType("hatred");
	RelationTypes::print();


}