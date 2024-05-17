#ifndef VN_OBSERVER_H
#define VN_OBSERVER_H

class Observer {
public:
	Observer() {};

	virtual void update() = 0;
};

#endif // VN_OBSERVER_H