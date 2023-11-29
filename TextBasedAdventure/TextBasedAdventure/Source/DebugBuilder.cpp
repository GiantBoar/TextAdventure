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
	DialogueNode newNode(dialogue, id, rootID);
	newNode.options = std::vector<int>();

	tree->tree.push_back(newNode);
	tree->IDIndex.insert({ id, tree->tree.size() - 1 });

	return id;
}

void SaveDialogueTree(DialogueTree* tree, std::string fileName)
{
	std::ofstream dialogueFile(ResourcePath("Dialogue/" + fileName), std::ifstream::binary | std::ofstream::trunc);
	Json::Value data, branch, option;
	DialogueNode node;

	for (int i = 0; i < tree->tree.size(); i++)
	{
		node = tree->tree[i];

		branch["ID"] = node.id;
		branch["dialogue"] = node.dialogue;
		branch["rootID"] = node.rootNodeID;

		for (int j = 0; j < node.options.size(); j++)
		{
			//option["ID"] = node.options[j].nextID;
			//option["dialogue"] = node.options[j].dialogue;
			branch["options"][j] = option;
		}

		data[i] = branch;
	}

	dialogueFile << data;

	dialogueFile.close();
}

void Debug::MakeDialogueTree()
{
	system("cls");

	std::cout << "name dialogue tree\n> ";
	std::string treeName;
	std::getline(std::cin, treeName);
	treeName += ".json";

	DialogueTree tree = DialogueTree();

	std::string command;
	int currentNodeID = -1;

	currentNodeID = CreateNode(&tree, -1);

	while (true)
	{
		system("cls");

		DialogueNode* node = tree.GetNode(currentNodeID);

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

			currentNodeID = CreateNode(&tree, lastDialogueID);

			DialogueOption newOption(lastDialogueID, optionText);

			//if (node->options.size() == 0) node->options = std::vector<DialogueOption>();

			//node->options.push_back(newOption);\

			node->options.push_back(lastDialogueID);
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
			SaveDialogueTree(&tree, treeName);
		}
	}

	SaveDialogueTree(&tree, treeName);
}