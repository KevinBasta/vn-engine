
#ifndef VN_ID_H
#define VN_ID_H

using id = int;

template <class T>
class IdGenerator {
private:
	static id m_currentId;

public:
	IdGenerator() = delete;

	static void setId(id i) {
		if (i > m_currentId) {
			m_currentId = i;
		}
	}

	static void setIdToAfter(id i) {
		if (i + 1 > m_currentId) {
			m_currentId = i + 1;
		}
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

#endif // VN_ID_H
