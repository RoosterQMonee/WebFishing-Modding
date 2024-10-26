#pragma once

#include <chrono>

class Misc {
public:
	static long long CurrentMs() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}
};