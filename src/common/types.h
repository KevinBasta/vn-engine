#ifndef VN_TYPES_H
#define VN_TYPES_H

//
// INDEX
// For use in
//
using index = int;

//
// ID
// For use in
//
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

	static const id returnGreatestId() {
		return m_currentId;
	}

	static const id getId() {
		return m_currentId++;
	}
};

template <class T>
int IdGenerator<T>::m_currentId{ 1 };

#endif // VN_TYPES_H