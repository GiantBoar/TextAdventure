#include "../Headers/DebugBuilder.h"

#pragma region DialogueTree

static int idCounter = 0;
int Debug::GetID()
{
	return ++idCounter;
}
int Debug::CreateNode(DialogueTree* tree, int rootID)
{
	std::string dialogue;
	std::cout << "dialogue for node\n> ";
	std::getline(std::cin, dialogue);

	int id = GetID();
	tree->tree.push_back(DialogueNode(dialogue, id, rootID));
	tree->IDIndex.insert({ id, tree->tree.size() - 1 });

	return id;
}
void Debug::SaveDialogueTree(DialogueTree* tree, std::string fileName)
{
	std::ofstream dialogueFile(ResourcePath("Dialogue/" + fileName), std::ifstream::binary | std::ofstream::trunc);
	Json::Value data, branchTree, branch, option;
	DialogueNode node;

	if (!dialogueFile.good()) printf(" but something went wrong!");

	data["characterName"] = tree->characterName;

	for (int i = 0; i < tree->tree.size(); i++)
	{
		node = tree->tree[i];
		
		branch = Json::Value();
		option = Json::Value();

		branch["ID"] = node.id;
		branch["dialogue"] = node.dialogue;
		branch["rootID"] = node.rootNodeID;

		for (int j = 0; j < node.options.size(); j++)
		{
			option["ID"] = node.options[j].first;
			option["dialogue"] = node.options[j].second;
			branch["options"][j] = option;
		}

		branchTree[i] = branch;
	}

	data["tree"] = branchTree;

	dialogueFile << data;

	dialogueFile.close();
}
bool Debug::CheckTree(DialogueTree* tree, DialogueNode* rootNode)
{
	if (rootNode->dialogue == "END" || rootNode->dialogue == "BATTLE") return true;
	else if (rootNode->options.size() == 0)
	{
		printf("error root: node %d dialogue %s", rootNode->id, rootNode->dialogue.c_str());
		return false;
	}

	for (int i = 0; i < rootNode->options.size(); i++)
	{
		if (!CheckTree(tree, tree->GetNode(rootNode->options[i].first)))
		{
			printf("error branch upwards: node %d with dialogue %s", rootNode->id, rootNode->dialogue.c_str());
			return false;
		}
	}
}

void Debug::DialogueTreeCommand(DialogueTree* tree, std::string treeName)
{
	int currentNodeID = tree->tree[0].id;

	std::string command;

	while (true)
	{
		system("cls");

		DialogueNode* node = tree->GetNode(currentNodeID);

		printf("input edit command for tree %s\ncurrent node: %d root node: %d \ndialogue: %s\noptions: %s\n> ", treeName.c_str(), currentNodeID, node->rootNodeID, node->dialogue.c_str(), node->OptionsList().c_str());
		std::getline(std::cin, command);

		if (command == "exit") break;

		if (command == "createoption")
		{
			if (node->dialogue == "END")
			{
				std::cout << "node is end node";
				continue;
			}

			int lastDialogueID = currentNodeID;

			std::cout << "input dialogue for option:\n> ";
			std::string optionText;
			std::getline(std::cin, optionText);

			currentNodeID = CreateNode(tree, lastDialogueID);

			tree->GetNode(lastDialogueID)->options.push_back({ currentNodeID, optionText });
		}
		else if (command == "back")
		{
			if (node->rootNodeID == -1)
			{
				std::cout << "node is root node, cant go back";
				Sleep(500);
				continue;
			}

			currentNodeID = node->rootNodeID;
		}
		else if (command == "edit")
		{
			std::cout << "enter new text\n> ";
			std::string newText;
			std::getline(std::cin, newText);

			node->dialogue = newText;
		}
		else if (command == "save")
		{
			SaveDialogueTree(tree, treeName);
		}
		else if (command == "root")
		{
			std::cout << "returnign to the root node";
			Sleep(1000);
			while (node->rootNodeID != -1)
			{
				node = tree->GetNode(node->rootNodeID);
				currentNodeID = node->id;
			}
		}
		else if (command.substr(0, 6) == "choose")
		{
			std::string option = command.substr(7, 8);

			if (!std::isdigit(option[0])) continue;

			int f = std::stoi(option);

			if (f >= node->options.size()) continue;

			currentNodeID = node->options[f].first;
		}
		else if(command == "checktree")
		{
			DialogueNode* n = node;

			while (n->rootNodeID != -1)
			{
				n = tree->GetNode(n->rootNodeID);
			}

			CheckTree(tree, n);

			std::getline(std::cin, command);
		}

		SaveDialogueTree(tree, treeName);
	}
}

void Debug::MakeDialogueTree()
{
	system("cls");

	std::cout << "name dialogue tree\n> ";
	std::string treeName, tempName;
	std::getline(std::cin, tempName);
	for (int i = 0; i < tempName.length(); i++) if (tempName[i] != ' ') treeName.push_back(tempName[i]);
	std::string characterName = treeName;
	treeName += ".json";

	DialogueTree tree = DialogueTree();
	tree.characterName = characterName;
	CreateNode(&tree, -1);

	DialogueTreeCommand(&tree, treeName);
}

void Debug::EditDialogueTree()
{
	DialogueTree t = DialogueTree();
	std::string filename;

	while (true)
	{
		system("cls");
		std::cout << "what is the name of your file!\n> ";
		std::getline(std::cin, filename);

		std::ifstream dialogueFile(ResourcePath("Dialogue/" + filename), std::ifstream::binary);
		if (!dialogueFile.good())
		{
			std::cout << "file does not exist!";
			dialogueFile.close();

			Sleep(1000);
			continue;
		}

		dialogueFile.close();

		t.LoadDialogueTree(filename);

		break;
	}

	DialogueTreeCommand(&t, filename);
}

#pragma endregion

void Debug::MakeLevel()
{
	system("cls");

	std::cout << "name level\n> ";
	std::string levelName, tempName;
	std::getline(std::cin, tempName);
	for (int i = 0; i < tempName.length(); i++) if (tempName[i] != ' ') levelName.push_back(tempName[i]);
	levelName += ".json";

	LevelData levelData = LevelData();

	std::string temp;

	std::cout << "enter the levels proper name:\n> ";
	std::getline(std::cin, temp);
	levelData.name = temp;

	std::cout << "enter the levels entrance description:\n> ";
	std::getline(std::cin, temp);
	levelData.description = temp;

	while (true)
	{
		std::cout << "what number index is the level?\n> ";
		std::string num;
		std::getline(std::cin, num);

		for (int i = 0; i < num.size(); i++)
		{
			if (!std::isdigit(num[i]))
			{
				std::cout << "input error\n";
				continue;
			}
		}

		int index = std::stoi(num);
		levelData.areaCode = index;
	}

	std::cout << "enter the levels help text:\n> ";
	std::getline(std::cin, temp);
	levelData.helptext = temp;


	std::cout << "enter the levels default look:\n> ";
	std::getline(std::cin, temp);
	levelData.lookInfo.insert({ "default", temp });

	LevelCommand(&levelData, levelName);
}

void Debug::SaveLevel(LevelData* levelData, std::string fileName)
{
	std::ofstream levelFile(ResourcePath("Levels/" + fileName), std::ifstream::binary | std::ofstream::trunc);
	Json::Value data, people, places, looks;

	if (!levelFile.good()) printf(" but something went wrong!");

	data["name"] = levelData->name;
	data["description"] = levelData->description;

	data["special"] = levelData->specialArea;
	data["areaCode"] = levelData->areaCode;
	data["help"] = levelData->helptext;
	data["lastLevel"] = levelData->lastLevelName;

	for (int i = 0; i < levelData->people.size(); i++)
	{
		people[i] = levelData->people[i];
	}
	for (int i = 0; i < levelData->places.size(); i++)
	{
		places[i] = levelData->places[i];
	}
	for (const auto& kvpair : levelData->lookInfo)
	{
		looks[kvpair.first] = kvpair.second;
	}

	data["people"] = people;
	data["places"] = places;
	data["lookInfo"] = looks;

	
	levelFile << data;

	levelFile.close();
}

void Debug::LevelCommand(LevelData* levelData, std::string levelName)
{
	std::string command;

	while (true)
	{
		system("cls");
		std::cout << "enter a command:\n> ";
		std::getline(std::cin, command);

		if (command == "createperson")
		{
			std::cout << "enter their name\n> ";
			std::getline(std::cin, command);

			levelData->people.push_back(command);
		}
		else if (command == "createplace")
		{
			std::cout << "enter the location name\n> ";
			std::getline(std::cin, command);

			levelData->places.push_back(command);
		}
		else if (command == "createlook")
		{
			std::cout << "enter its name\n> ";
			std::getline(std::cin, command);

			std::cout << "enter its description\n> ";
			std::string temp;
			std::getline(std::cin, temp);

			levelData->lookInfo.insert({ command, temp });
		}
		else if (command == "toggleAreaSpecial")
		{
			std::cout << !levelData->specialArea ? "area is now special" : "area is not special";
			levelData->specialArea = !levelData->specialArea;

			Sleep(2000);
		}
		else if (command == "setlastlevel")
		{
			std::cout << "enter the name of the last level\n> ";
			std::getline(std::cin, command);

			levelData->lastLevelName = command;
		}
		else if (command == "exit")
		{
			break;
		}

		Debug::SaveLevel(levelData, levelName);
	}
}