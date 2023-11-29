#pragma once

#include <unordered_map>
#include <fstream>

#include "Json/Json.h"
#include "Json/json-forwards.h"
#include "ResourcePath.h"

// smaller class used to store individual 'branches' of dialogue, such as person says something and user is prompted for an answer
class DialogueNode
{
	int id;
	std::string dialogue;
	std::vector<std::pair<int, std::string>> options;
};

class DialogueTree
{
	std::vector<DialogueNode> tree;
	int currentNode;

	void LoadDialogueTree(std::string path);
};