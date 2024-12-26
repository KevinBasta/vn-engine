#include <iostream>
#include <functional>

class NodeAction {
public:
    template <class T>
    NodeAction(T obj) {
        // Create a lambda function that returns the value of the object.
        get = [=]() { return obj; };
    }

// private:
//     class Temp {
//         virtual 
//     }

    
};

int main() {
    NodeAction action{1};
    
    return 0;
}
