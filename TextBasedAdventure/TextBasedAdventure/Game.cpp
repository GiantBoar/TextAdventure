#include "Game.h"

long long GetCurrentTime()
{
	return std::chrono::steady_clock::now().time_since_epoch().count();
}