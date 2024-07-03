#ifndef VN_SUBJECT_H
#define VN_SUBJECT_H

#include "observer.h"

#include <list>
#include <vector>
#include <iterator>
#include <iostream>

class Subject {
private:
	std::list<Observer*> m_observers{};

public:
	Subject() {}
	virtual ~Subject() {}

	void attatch(Observer* observer) {
		if (observer != nullptr) {
			m_observers.push_back(observer);
			std::cout << "attatched" << std::endl;
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
		for (std::list<Observer*>::iterator iter{ m_observers.begin() }; iter != m_observers.end(); ++iter) {
			if (*iter != nullptr) {
				(*iter)->update();
			}
		}
		std::cout << "m_observers address: " << &m_observers << std::endl;
		std::cout << "notification occured for " << m_observers.size() << " observers" << std::endl;
	}
};

#endif // VN_SUBJECT_H