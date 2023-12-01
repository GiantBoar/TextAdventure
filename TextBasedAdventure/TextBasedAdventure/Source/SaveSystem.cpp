#include "../Headers/SaveSystem.h"

void SaveSystem::SavePlayerData(PlayerData* data)
{
	std::ofstream playerFile(ResourcePath("player.json"), std::ofstream::binary | std::ofstream::trunc);
	Json::Value playerData;
	Json::Value flagMap;


	playerData["name"] = data->name;
	playerData["characterClass"] = data->characterClass;

	for (const auto& kvpair : data->flags)
	{
		flagMap[kvpair.first] = kvpair.second;
	}

	playerData["flags"] = flagMap;

	playerData["location"] = data->currentLocation;


	playerFile << playerData;
	playerFile.close();
}

void SaveSystem::LoadPlayerData(PlayerData* data)
{
	std::ifstream playerFile(ResourcePath("player.json"), std::ifstream::binary);
	Json::Value playerData;

	if (!playerFile.good()) return;

	playerFile >> playerData;

	data->name = playerData["name"].asString();
	data->characterClass = playerData["characterClass"].asString();

	data->currentLocation = playerData["location"].asString();

	data->flags.clear();
	std::string key;
	for (int i = 0; i < playerData["keys"].size(); i++)
	{
		key = playerData["keys"].getMemberNames()[i];
		data->flags[key] = playerData["keys"][key].asBool();
	}

	playerFile.close();
}

bool SaveSystem::CanLoadSave()
{
	std::ifstream playerFile(ResourcePath("player.json"));
	bool good = playerFile.good();
	playerFile.close();
	return good;
}