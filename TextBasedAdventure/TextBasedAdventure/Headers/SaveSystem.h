#pragma once

#define JSON_IS_AMALGAMATION

#include <fstream>
#include <iostream>
#include <map>

#include "Json/json.h"
#include "Json/json-forwards.h"

#include "ResourcePath.h"

// forward decleration
enum class PlayerAttacks;
enum class PlayerSkills;
enum class GameState;

namespace SaveSystem
{
	// class used to store all the player's data
	class PlayerData
	{
	public:
		std::string name;
		GameState currentLocation = (GameState)0;

		std::vector<PlayerAttacks> attacks;
		std::vector<PlayerSkills> skills;

		// collection of flags for events / progression
		std::map<std::string, bool> flags =
		{
			{ "SurvivedForest", false },
			{ "Test", false }
		};
	};

	void SavePlayerData(PlayerData* data);
	void LoadPlayerData(PlayerData* data);

	bool CanLoadSave();
}