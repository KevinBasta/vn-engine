#ifndef VN_SUBJECT_H
#define VN_SUBJECT_H

#include "observer.h"

#include <list>
#include <vector>
#include <iterator>
#include <iostream>

class Subject {
private:
	std::vector<Observer*> m_observers{};

public:
	Subject() {}
	virtual ~Subject() {}

	void attatch(Observer* observer) {
		if (observer != nullptr) {
			m_observers.push_back(observer);
			m_observers.begin();
		std::cout << "attatched" << std::endl;
		}
		std::cout << "attatched" << std::endl;
	}

	void detatch(Observer* observer) {
		if (observer == nullptr) {
			return;
		}
		
		/*for (std::list<Observer*>::iterator iter{ m_observers.begin() }; iter != m_observers.end(); iter++) {
			if (observer == *iter) {
				m_observers.erase(iter);
			}
		}*/
	}
	
	void notify() {
		//std::cout << m_observers.begin() << std::endl;
		//for (std::list<Observer*>::iterator iter = m_observers.begin(); iter != m_observers.end(); ++iter) {
			/*if (*iter != nullptr) {
				(*iter)->update();
			}*/
		//}
		std::cout << "notification occured for " << &m_observers << " observers" << std::endl;
		std::cout << "notification occured for " << m_observers.size() << " observers" << std::endl;
	}
};

#endif // VN_SUBJECT_H