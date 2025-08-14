
#ifndef VN_ID_H
#define VN_ID_H

#include <set>

using id = int;

template <class T>
class IdGenerator {
private:
	static id m_currentId;
	static std::set<id> m_deleted;

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
		if (!m_deleted.empty()) {
			id first{ *(m_deleted.begin()) };
			m_deleted.erase(m_deleted.begin());
			return first;
		}

		return m_currentId++;
	}

	static void deleted(id i) {
		m_deleted.insert(i);
	}
};

template <class T>
int IdGenerator<T>::m_currentId{ 1 };

template <class T>
std::set<id> IdGenerator<T>::m_deleted{};

#endif // VN_ID_H
