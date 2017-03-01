#ifndef WANDERINGIPS_APP_HPP_
#define WANDERINGIPS_APP_HPP_

#include <adbase/Config.hpp>
#include "AdbaseConfig.hpp"


class App {
public:
	App(AdbaseConfig* config);
	~App();
	void reload();
	void run();
	void setAdServerContext(AdServerContext* context);
	void setTimerContext(TimerContext* context);
	void loadConfig(adbase::IniConfig& config);
	uint64_t getSeqId();

private:
	AdbaseConfig* _configure;
	mutable std::mutex _mut;
	BinarySearch* _bs;
};

#endif
