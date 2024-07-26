
#ifndef VN_ID_H
#define VN_ID_H

using id = int;

template <class T>
class IdGenerator {
private:
	static id m_currentId;

public:
	IdGenerator() = delete;

	static void setId(int i) {
		m_currentId = i;
	}

	static const int getId() {
		return m_currentId++;
	}
};

template <class T>
int IdGenerator<T>::m_currentId{ 1 };

#endif // VN_ID_H
