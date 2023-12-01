#pragma once

#include <unordered_map>
#include <fstream>

#include "Json/Json.h"
#include "Json/json-forwards.h"
#include "ResourcePath.h"

#include "Graphics.h"

void LoadCurrentLevel();
void LoadLevel(std::string levelName);

// smaller struct used to store individual 'branches' of dialogue, such as person says something and user is prompted for an answer
struct DialogueNode
{
	int id, rootNodeID;
	std::string dialogue;
	std::vector<std::pair<int, std::string>> options;

	DialogueNode(std::string dialogue, int ID, int rootID);
	DialogueNode();

	std::string OptionsList();
};

class DialogueTree
{
public:
	std::string characterName = "";

	std::vector<DialogueNode> tree;
	std::unordered_map<int, int> IDIndex;
	int currentNode;

	void LoadDialogueTree(std::string path);

	DialogueNode* GetNode(int id);
	DialogueNode* GetCurrentNode();
	void ChooseOption(int optionIndex);
};

void DrawNode();
void SelectDialogueOption();

extern DialogueTree* currentTree;