#ifndef VN_SUBJECT_H
#define VN_SUBJECT_H

#include "observer.h"

#include <list>
#include <iostream>

class Subject {
private:
	std::list<Observer*> m_observers{};

public:
	void attatch(Observer* observer) {
		if (observer != nullptr) {
			m_observers.push_back(observer);
		}
	}

	void detatch(Observer* observer) {
		if (observer == nullptr) {
			return;
		}
		
		for (std::list<Observer*>::iterator iter{ m_observers.begin() }; iter != m_observers.end(); iter++) {
			if (observer == *iter) {
				m_observers.erase(iter);
			}
		}
	}
	
	void notify() {
		for (std::list<Observer*>::iterator iter{ m_observers.begin()}; iter != m_observers.end(); iter++) {
			(*iter)->update();
		}

		std::cout << "notification occured for " << m_observers.size() << " observers" << std::endl;
	}
};

#endif // VN_SUBJECT_H