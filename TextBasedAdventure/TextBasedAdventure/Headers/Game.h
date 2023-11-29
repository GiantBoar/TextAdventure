#pragma once

#include <chrono>
#include <iostream>
#include <windows.h>
#include <cmath>

#include "Graphics.h"
#include "Input.h"
#include "SaveSystem.h"
#include "Combat.h"

extern class GraphicsHandler* graphics;
extern class InputHandler* inputs;

extern class SaveSystem::PlayerData* playerData;

extern bool gameRunning;

// enum for all possible game states
enum class GameState
{
	// menus
	Title,
	Credits,
	Pause,

	// levels
	OpeningCutscene
};

void GameLoop();
void SetupInputs();
void SelectCharacter();

std::clock_t GetTime();