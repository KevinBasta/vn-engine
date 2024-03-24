#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <iostream>

class Timer {
private:
	// Type aliases to make accessing nested type easier
	using Clock = std::chrono::steady_clock;
	using Second = std::chrono::duration<double, std::ratio<1> >;

	std::chrono::time_point<Clock> start { Clock::now() };

public:
	void reset() {
		start = Clock::now();
	}

	double elapsed() const {
		return std::chrono::duration_cast<Second>(Clock::now() - start).count();
	}

	void resetp() {
		reset();
		std::cout << "============ Timer Started ============" << std::endl;
	}

	void elapsedp() const {
		std::cout << "====== Elapsed: " << elapsed() << " ======" << std::endl;
	}
};

#endif // TIMER_H