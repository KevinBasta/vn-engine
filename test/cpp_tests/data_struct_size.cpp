#include <iostream>
#include <unordered_map>
#include <vector>
#include <optional>

int main() {
	std::unordered_map<int, std::vector<int>> map{};
	std::vector<std::optional<std::vector<int>>> vec{};

	std::cout << sizeof(map) << std::endl;
	std::cout << sizeof(vec) << std::endl;
	
	map[4] = { 1, 2 };

	if (vec.capacity() < 4) {
		vec.reserve(5);
	}

	vec[4].emplace({ 1, 2 });

	std::cout << sizeof(map) << std::endl;
	std::cout << sizeof(vec) << std::endl;
}