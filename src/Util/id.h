
class Id {
private:
	const int m_id;

public:
	Id(int n) : m_id{ n } { }
	
	const int get() const {
		return m_id;
	}
	
	operator int() const {
		return m_id;
	}
};

static class IdGenerator {
private:
	static int currentId;

public:
	static Id getId() {
		return Id(currentId++);
	}
};

int IdGenerator::currentId{ 1 };
