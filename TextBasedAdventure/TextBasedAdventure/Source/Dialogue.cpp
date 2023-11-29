#include "../Headers/Dialogue.h"

void DialogueTree::LoadDialogueTree(std::string fileName)
{
	std::ifstream dialogueFile(ResourcePath("Dialogue/" + fileName), std::ifstream::binary);
	Json::Value data;

	if (!dialogueFile.good()) return;

	dialogueFile >> data;

	int branchNum = data.size();

	Json::Value option;

	for (int i = 0; i < branchNum; i++)
	{
		int currentNodeID = data[i]["ID"].asInt();

		tree.push_back(DialogueNode(data[i]["dialogue"].asString(), currentNodeID, data[i]["rootID"].asInt()));
		IDIndex.insert({ currentNodeID, tree.size() - 1 });

		int optionsNum = data[i]["options"].size();
		for (int j = 0; j < optionsNum; j++)
		{
			option = data[i]["options"][j];
			//GetNode(currentNodeID)->options.push_back({ option["ID"].asInt(), option["dialogue"].asString() });
		}
	}

	dialogueFile.close();
}

DialogueNode::DialogueNode(std::string dialogue, int id, int rootID)
{
	this->dialogue = dialogue;
	this->id = id;
	this->rootNodeID = rootID;

	options = std::vector<int>();
}
DialogueNode::DialogueNode()
{
	this->dialogue = "EMPTY";
	this->id = -10;
	this->rootNodeID = -10;
}

void DialogueNode::AddOption(int o)
{
	options.push_back(o);
}

std::string DialogueNode::OptionsList()
{
	std::string s = "";

	printf("size of thing is %d DONT TEST ME", options.size());

	for (int i = 0; i < options.size(); i++)
	{
		//s += options[i].dialogue;

		if (i != options.size() - 1) s += ", ";
	}
	return s;
}

DialogueNode* DialogueTree::GetNode(int id)
{
	return &tree[IDIndex[id]];
}