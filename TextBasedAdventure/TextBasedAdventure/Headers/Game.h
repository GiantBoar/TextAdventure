#pragma once

#include <chrono>
#include <iostream>
#include <windows.h>
#include <cmath>

#include "Graphics.h"
#include "Input.h"
#include "SaveSystem.h"

#define DEBUG_BUILD false

#if DEBUG_BUILD
#include "DebugBuilder.h"
#else
#include "Dialogue.h"
#include "Level.h";
#endif

extern class GraphicsHandler* graphics;
extern class InputHandler* inputs;
extern class SaveSystem::PlayerData* playerData;
extern class LevelData* currentLevel;

extern class SaveSystem::PlayerData* playerData;

extern bool gameRunning;

// enum for all possible game states
enum class GameState
{
	// menus
	Title,
	Credits,
	Pause,
	Dialogue,
	// levels
	OpeningCutscene,
	DefaultLevel,
	Campfire,

	ForestEntrance,
	ForestPath,
	ForestExit,

	Town
};

void GameLoop();
void ProcessInput();
void SetupInputs();
std::clock_t GetTime();

void EndGame();
void ContinueGame();
COORD GetInputPosition();

void ForestCommands();

void ChangeState(GameState newState);
void LoadLevel(std::string levelName);
void LevelCommand();
void DefaultLevelCommands(Command* c);
void LoadDialogue(std::string characterName);

void SelectCharacter();

std::clock_t GetTime();