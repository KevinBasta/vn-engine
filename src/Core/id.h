
#ifndef ID_H
#define ID_H

enum class IdType {

};


class IdGenerator {
private:
	// maybe id variable for each entity type that needs and id
	static int m_currentId;

public:
	IdGenerator() = delete;

	static const int getId() {
		return m_currentId++;
	}
};

#endif // ID_H
