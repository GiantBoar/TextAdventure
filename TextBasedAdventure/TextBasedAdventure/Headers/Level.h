#pragma once

#include <string>
#include <unordered_map>
#include <fstream>

#include "Json/Json.h"
#include "Json/json-forwards.h"
#include "ResourcePath.h"

struct LevelData
{
	std::string name = "", description = "", helptext = "", lastLevelName = "";
	std::unordered_map<std::string, std::string> lookInfo = std::unordered_map<std::string, std::string>();
	std::vector<std::string> people = std::vector<std::string>();
	std::vector<std::string> places = std::vector<std::string>();
	bool specialArea = false;
	int areaCode = 0;
};

void LoadLevelData(LevelData* levelData, std::string fileName);