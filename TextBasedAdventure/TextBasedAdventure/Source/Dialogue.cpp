#include "../Headers/Dialogue.h"

void DialogueTree::LoadDialogueTree(std::string fileName)
{
	std::ifstream dialogueFile(ResourcePath("Dialogue/" + fileName), std::ifstream::binary);

	Json::Value data, branchTree;

	if (!dialogueFile.good()) return;

	dialogueFile >> data;

	branchTree = data["tree"];

	int branchNum = branchTree.size();

	Json::Value option;

	for (int i = 0; i < branchNum; i++)
	{
		int currentNodeID = branchTree[i]["ID"].asInt();

		tree.push_back(DialogueNode(branchTree[i]["dialogue"].asString(), currentNodeID, branchTree[i]["rootID"].asInt()));
		IDIndex.insert({ currentNodeID, tree.size() - 1 });

		int optionsNum = branchTree[i]["options"].size();
		for (int j = 0; j < optionsNum; j++)
		{
			option = branchTree[i]["options"][j];
			GetNode(currentNodeID)->options.push_back({ option["ID"].asInt(), option["dialogue"].asString() });
		}
	}

	characterName = data["characterName"].asString();

	currentNode = tree[0].id;

	dialogueFile.close();
}

DialogueNode::DialogueNode(std::string dialogue, int id, int rootID)
{
	this->dialogue = dialogue;
	this->id = id;
	this->rootNodeID = rootID;

	options = std::vector<std::pair<int, std::string>>();
}
DialogueNode::DialogueNode()
{
	this->dialogue = "EMPTY";
	this->id = -10;
	this->rootNodeID = -10;

	options = std::vector<std::pair<int, std::string>>();
}

std::string DialogueNode::OptionsList()
{
	std::string s = "";

	for (int i = 0; i < options.size(); i++)
	{
		s += options[i].second;

		if (i != options.size() - 1) s += ", ";
	}

	return s;
}

DialogueNode* DialogueTree::GetNode(int id)
{
	return &tree[IDIndex[id]];
}
DialogueNode* DialogueTree::GetCurrentNode()
{
	return GetNode(currentNode);
}

void DialogueTree::ChooseOption(int optionIndex)
{
	GraphicsHandler* graphics = GraphicsHandler::GetInstance();

	DialogueNode* node = GetCurrentNode();

	graphics->buttons.clear();

	if (optionIndex >= node->options.size()) return;

	graphics->Redraw();

	graphics->WriteLine(node->options[optionIndex].second);

	currentNode = node->options[optionIndex].first;
	DrawNode();
}

DialogueTree* currentTree = new DialogueTree;

void DrawNode()
{
	GraphicsHandler* graphics = GraphicsHandler::GetInstance();
	DialogueNode* node = currentTree->GetCurrentNode();

	if (node->dialogue.substr(0, 6) == "BATTLE" || node->dialogue.substr(0, 3) == "END")
	{
		std::vector<std::string> args;
		int currentArg = 0;

		args.push_back(std::string());
		for (int i = 0; i < node->dialogue.length(); i++)
		{
			if (node->dialogue[i] == ' ')
			{
				args.push_back(std::string());
				currentArg++;
			}
			else
			{
				args[currentArg].push_back(node->dialogue[i]);
			}
		}

		if (args[0] == "BATTLE")
		{

		}
		else if (args[0] == "END")
		{
			if (args.size() == 1) LoadCurrentLevel();
			else
			{
				LoadLevel(args[1] + ".json");
			}
		}

		return;
	}

	std::vector<UI::Button> options;

	graphics->Reset();

	graphics->cacheOffset = -2;
	graphics->AddDivider(graphics->windowSize.Y - 5);

	for (int i = 0; i < node->options.size(); i++)
	{
		options.push_back(UI::Button(ScreenCoord(1, 1), " " + node->options[i].second + " ", false, SelectDialogueOption));
	}

	graphics->Redraw();

	Sleep(1000);

	graphics->WriteLine(GraphicsHandler::ColourString(currentTree->characterName + ": ", COLOUR_BRIGHT(COLOUR_YELLOW)) + node->dialogue);
	graphics->OrganiseButtons(ScreenCoord(2, graphics->windowSize.Y - 3), ScreenCoord(0, 1), options);
}

void SelectDialogueOption()
{
	GraphicsHandler* graphics = GraphicsHandler::GetInstance();

	int index = graphics->selectedButtonIndex;

	std::cout << index;

	currentTree->ChooseOption(index);
}