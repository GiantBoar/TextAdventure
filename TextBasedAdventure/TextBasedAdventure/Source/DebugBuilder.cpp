#include "../Headers/DebugBuilder.h"

static int idCounter = 0;
int GetID()
{
	return ++idCounter;
}

int CreateNode(DialogueTree* tree, int rootID)
{
	std::string dialogue;
	std::cout << "dialogue for node\n> ";
	std::getline(std::cin, dialogue);

	int id = GetID();
	tree->tree.push_back(DialogueNode(dialogue, id, rootID));
	tree->IDIndex.insert({ id, tree->tree.size() - 1 });

	return id;
}

void SaveDialogueTree(DialogueTree* tree, std::string fileName)
{
	std::ofstream dialogueFile(ResourcePath("Dialogue/" + fileName), std::ifstream::binary | std::ofstream::trunc);
	Json::Value data, branch, option;
	DialogueNode node;

	if (!dialogueFile.good()) printf(" but something went wrong!");

	for (int i = 0; i < tree->tree.size(); i++)
	{
		node = tree->tree[i];

		branch["ID"] = node.id;
		branch["dialogue"] = node.dialogue;
		branch["rootID"] = node.rootNodeID;

		for (int j = 0; j < node.options.size(); j++)
		{
			option["ID"] = node.options[j].first;
			option["dialogue"] = node.options[j].second;
			branch["options"][j] = option;
		}

		data[i] = branch;
	}

	dialogueFile << data;

	dialogueFile.close();
}

void DialogueTreeCommand(DialogueTree* tree, std::string treeName)
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
			while (node->rootNodeID != -1)
			{
				node = tree->GetNode(node->rootNodeID);
			}
		}
		else if (command.substr(0, 6) == "choose")
		{
			std::string option = command.substr(7, 8);

			if (!std::isdigit(option[0])) continue;

			int f = std::stoi(option);

			if (f >= node->options.size()) continue;

			currentNodeID = node->options[f].first;
			std::cout << node->options[f].first;
			Sleep(1000);
		}
	}
}

void Debug::MakeDialogueTree()
{
	system("cls");

	std::cout << "name dialogue tree\n> ";
	std::string treeName;
	std::getline(std::cin, treeName);
	treeName += ".json";

	DialogueTree tree = DialogueTree();
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