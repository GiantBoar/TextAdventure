#pragma once

#include <unordered_map>
#include <fstream>

// delte
#include <iostream>
#include <Windows.h>

#include "Json/Json.h"
#include "Json/json-forwards.h"
#include "ResourcePath.h"

// smaller struct used to store individual 'branches' of dialogue, such as person says something and user is prompted for an answer
struct DialogueNode
{
	int id, rootNodeID;
	std::string dialogue;
	std::vector<int> options;

	DialogueNode(std::string dialogue, int ID, int rootID);
	DialogueNode();

	void AddOption(int o);

	std::string OptionsList();
};

class DialogueTree
{
public:
	std::vector<DialogueNode> tree;
	std::unordered_map<int, int> IDIndex;
	int currentNode;

	void LoadDialogueTree(std::string path);

	DialogueNode* GetNode(int id);
};