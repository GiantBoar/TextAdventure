#include "../Headers/Level.h"

void LoadLevelData(LevelData* levelData, std::string fileName)
{
	std::ifstream levelFile(ResourcePath("Levels/" + fileName), std::ifstream::binary);
	Json::Value data;

	if (!levelFile.good()) return;

	levelFile >> data;

	std::string key;

	levelData->name = data["name"].asString();
	levelData->description = data["description"].asString();

	levelData->people.clear();
	for (int i = 0; i < data["people"].size(); i++)
	{
		levelData->people.push_back(data["people"][i].asString());
	}

	levelData->places.clear();
	for (int i = 0; i < data["places"].size(); i++)
	{
		levelData->places.push_back(data["places"][i].asString());
	}

	levelData->lookInfo.clear();
	for (int i = 0; i < data["lookInfo"].size(); i++)
	{
		key = data["lookInfo"].getMemberNames()[i];
		levelData->lookInfo.insert({ key, data["lookInfo"][key].asString() });
	}

	levelData->areaCode = data["areaCode"].asInt();
	levelData->specialArea = data["special"].asBool();

	levelData->helptext = data["help"].asString();

	levelData->lastLevelName = data["lastLevel"].asString();

	levelFile.close();
}