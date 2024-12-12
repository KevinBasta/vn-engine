
#include <iostream>

class Base {
private:
	static void test() { std::cout << "test" << std::endl; }
};

class Der : public Base {
public:
	static void impl() { std::cout << "impl" << std::endl; }
};


namespace BaseTwo {

}


int main() {

	//Der::test();



	return 0;
}