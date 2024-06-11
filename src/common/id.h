
#ifndef VN_ID_H
#define VN_ID_H

template <class T>
class IdGenerator {
private:
	static int m_currentId;

public:
	IdGenerator() = delete;
	IdGenerator(int i) : m_currentId{ i } {};

	static const int getId() {
		return m_currentId++;
	}
};

template <class T>
int IdGenerator<T>::m_currentId{ 1 };


#endif // VN_ID_H
