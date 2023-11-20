#pragma once

#include <chrono>
#include <iostream>
#include <windows.h>

#include "Graphics.h"
#include "Input.h"
#include "SaveSystem.h"

extern class GraphicsHandler* graphics;
extern class InputHandler* inputs;

extern class SaveSystem::PlayerData* playerData;

extern bool gameRunning;

// enum for all possible game states
enum class GameState
{
	Title,
	Credits,
	CharacterSelect
};

// enum for player abilities
enum class PlayerAttacks
{
	// starting attacks for each 3 classes
	Slash,
	Poke,
	Shoot,

	// additional attacks they can learn
	Stab, // does bleed damage
	Pummel, // makes them less agile
};

enum class PlayerSkills
{
	// starting skills for each 3 classes
	Heal,
	Lightning,
	Focus,

	// additional skills they can learn
	Parry, // if the enemy does an attack next turn, block and deal double their damage
};

void GameLoop();
void SetupInputs();

std::clock_t GetTime();