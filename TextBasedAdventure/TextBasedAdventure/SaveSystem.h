#pragma once

#define JSON_IS_AMALGAMATION

#include "json.h"
#include "json-forwards.h"

#include <fstream>
#include <iostream>
#include <map>

// forward decleration
enum class PlayerAttacks;
enum class PlayerSkills;

namespace SaveSystem
{
	// class used to store all the player's data
	class PlayerData
	{
	public:
		std::string name;
		std::string currentLocation;

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