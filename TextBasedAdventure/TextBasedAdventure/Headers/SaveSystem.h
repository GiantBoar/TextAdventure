#pragma once

#define JSON_IS_AMALGAMATION

#include <fstream>
#include <iostream>
#include <map>

#include "Json/json.h"
#include "Json/json-forwards.h"

#include "ResourcePath.h"

// forward decleration
struct PlayerAttacks;
struct PlayerSkills;
enum class GameState;

namespace SaveSystem
{
	// class used to store all the player's data
	class PlayerData
	{
	public:
		std::string name, characterClass;
		std::string currentLocation;

		// collection of flags for events / progression
		std::map<std::string, bool> flags =
		{
			{ "adventurer", false },
			{ "survivedForest", false }
		};
	};

	void SavePlayerData(PlayerData* data);
	void LoadPlayerData(PlayerData* data);

	bool CanLoadSave();
}