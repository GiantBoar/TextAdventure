#pragma once

#include <chrono>

#include "Graphics.h"
#include "Input.h"

class Time
{
public:
	static std::clock_t GetTime();
};

GraphicsHandler* graphics;