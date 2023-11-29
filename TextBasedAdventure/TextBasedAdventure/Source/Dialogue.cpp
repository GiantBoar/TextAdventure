#include "../Headers/Dialogue.h"

void DialogueTree::LoadDialogueTree(std::string fileName)
{
	std::ifstream dialogueFile(ResourcePath("Dialogue/" + fileName), std::ifstream::binary);
	Json::Value data;

	if (!dialogueFile.good()) return;

	dialogueFile >> data;



	dialogueFile.close();
}