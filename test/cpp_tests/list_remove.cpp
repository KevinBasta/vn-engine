#include <list>
#include <iostream>

void print(std::list<int>& list) {
	for (int n : list) {
		std::cout << n << " ";
	}

	std::cout << std::endl;
}


int main() {

	std::list<int> test { 1, 2, 3 };
	print(test);

	test.remove(5);
	print(test);

	test.remove(2);
	print(test);

	std::cout << "does that throw an exception?" << std::endl;
}
