
#include <vector>
#include <unordered_map>
#include <iostream>

std::unordered_map<int, std::vector<int>> ovec{
    {0, {1}}
};

std::vector<int>& getVec(int index) {
    return ovec[index];
}

int main() {
    //.. no error
    std::vector<int>& test = getVec(1);

    for (auto e : test) {
        std::cout << e << std::endl;
    }

    return 0;
}