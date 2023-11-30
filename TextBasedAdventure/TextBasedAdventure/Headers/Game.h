#pragma once

#include <chrono>
#include <iostream>
#include <windows.h>
#include <cmath>

#include "Graphics.h"
#include "Input.h"
#include "SaveSystem.h"
#include "Combat.h"

#define DEBUG_BUILD true

#if DEBUG_BUILD
#include "DebugBuilder.h"
#endif

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
	OpeningCutscene,
	DefaultLevel
};

void GameLoop();
void SetupInputs();
std::clock_t GetTime();

void EndGame();
void ContinueGame();
COORD GetInputPosition();

void ChangeState(GameState newState);
void LoadLevel(std::string levelName);
void LevelCommand();
void LoadDialogue(std::string characterName);

void SelectCharacter();

std::clock_t GetTime();