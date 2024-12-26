#include <iostream>

int main() {
    bool test = false;

    test |= true;
    test |= false;

    if (test) {
        std::cout << "true" << std::endl;
    } else {
        std::cout << "false" << std::endl;
    }

    return 0;
}