

#include <vector>
#include <hash_map> // replace with newer map
#include <utility>
#include <string>

/*
 * A generalized data class that contians some statuses (e.g. affection)
 * towards the main character. Can be generalized to include for all other characters?
 */

class Bond {
private:
	// bond name, bond id
	static std::vector<std::pair<std::string, int>> bondTypes;

	// maybe a weak pointer to a shared pointer that is the character?
	// character id: list(<bond id, bond level>)
	std::hash_map<int, std::vector<std::pair<int, int>>> bondLevel;

public: 
	Bond() : bondLevel{} {
	
	};

	static void addBondType(std::string bondName);
};
