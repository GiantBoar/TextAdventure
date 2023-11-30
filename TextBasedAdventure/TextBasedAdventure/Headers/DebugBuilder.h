#pragma once

#include <iostream>
#include <Windows.h>
#include <string>

#include "Dialogue.h"
#include "Level.h";

#include "Json/Json.h"
#include "Json/json-forwards.h"
#include "ResourcePath.h"

namespace Debug
{
	void MakeDialogueTree();
	void EditDialogueTree();

	void DialogueTreeCommand(DialogueTree* tree, std::string treeName);
	bool CheckTree(DialogueTree* tree, DialogueNode* rootNode);
	void SaveDialogueTree(DialogueTree* tree, std::string fileName);
	int CreateNode(DialogueTree* tree, int rootID);

	int GetID();

	void MakeLevel();
	void SaveLevel(LevelData* levelData, std::string fileName);
	void LevelCommand(LevelData* levelData, std::string fileName);
}