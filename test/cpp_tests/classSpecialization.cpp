#include <iostream>

template <class T>
class Base {
private: 
    T m_obj{};

public:
    T get() { return m_obj; }
    void set(T obj) { m_obj = obj; }
    virtual void draw() = 0; // The class specilization must redefine this or it will result in compile time error
    // = 0 means nothing without "virtual". with virutal it's pure virutal. compiles either way on g++ but without virutal is
    // compiler specific behaviour.
};

template<>
class Base<int> {
public:
    void draw() {
        std::cout << "drawing int" << std::endl;
    }
};

int main() {
    Base<int> t;
    t.draw();

    return 0;
}