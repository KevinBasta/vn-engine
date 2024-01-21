
#include <bond.h>

#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::string, int>> Bond::bondTypes{};

void Bond::addBondType(std::string bondName) {
	bondTypes.push_back(std::pair(bondName, static_cast<int>(bondTypes.size())));
}