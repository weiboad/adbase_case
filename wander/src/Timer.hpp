#ifndef WANDERINGIPS_TIMER_HPP_
#define WANDERINGIPS_TIMER_HPP_

#include <adbase/Utility.hpp>
#include <adbase/Logging.hpp>
#include <adbase/Net.hpp>
#include "AdbaseConfig.hpp"
#include "Wander/BinarySearch.hpp"

class Timer {
public:
	Timer(TimerContext* context);
	~Timer();
	void init();
	adbase::Timer* getTimer();
	void Test(void* args);

private:
	/// 传输上下文指针
	TimerContext* _context;
	AdbaseConfig* _configure;
	adbase::Timer* _timer;
	std::vector<uint64_t> _timerIds;
};

#endif
