#include "../Headers/Input.h"

// singleton instancing and constructor
InputHandler* InputHandler::_instance = nullptr;
InputHandler* InputHandler::GetInstance()
{
	if (_instance == NULL)
	{
		_instance = new InputHandler();
	}

	return _instance;
}
InputHandler::InputHandler()
{
	inputHandle = GetStdHandle(STD_INPUT_HANDLE);
}

void LowercaseString(std::string& str)
{
	for (int i = 0; i < str.length(); i++) str[i] = std::tolower(str[i]);
}

void InputHandler::ProcessInput()
{
	DWORD inpCount;
	GetNumberOfConsoleInputEvents(inputHandle, &inpCount);

	if (inpCount > 0)
	{
		ReadConsoleInput(inputHandle, &inputRecord, 1, &InputsRead);

		WORD currentInput = inputRecord.Event.KeyEvent.wVirtualKeyCode;

		if (keymap.find(currentInput) != keymap.end() && GetKeyState(currentInput) & KEY_PRESSED)
		{
			((void(*)(WORD))keymap[currentInput])(currentInput);
		}
	}

	if(inpCount > 0)
		FlushConsoleInputBuffer(inputHandle);
}


void InputHandler::AddKey(WORD key, void (* data)(WORD))
{
	if (keymap.find(key) != keymap.end()) return;

	keymap[key] = data;
}

void InputHandler::RemoveKey(WORD key)
{
	keymap.erase(key);
}


InputHandler::KeyData::KeyData(void* onPress)
{
	pressed = false;

	onKeyDown = onPress;
}

std::string InputHandler::GetRawInputString()
{
	COORD p = GetInputPosition();
	printf("\033[%d;%dH", p.Y, p.X);

	std::string input;
	std::getline(std::cin, input);
	return input;
}

std::string InputHandler::GetInputString()
{
	COORD p = GetInputPosition();
	printf("\033[%d;%dH", p.Y, p.X);

	std::string input;
	std::getline(std::cin, input);
	LowercaseString(input);

	bool checkVal = false;
	std::string checkString = input;

	// only checks the first part of the input
	if (splitInput)
	{
		checkString = "";
		for (int i = 0; i < input.length(); i++)
		{
			if (input[i] == ' ') break;

			checkString.push_back(input[i]);
		}
	}

	for (int i = 0; i < inputOptions.size(); i++)
	{
		if (inputOptions[i] == input)
		{
			checkVal = true;
			break;
		}
	}

	if (checkVal)
	{
		return input;
	}
	else 
	{
		return "ERROR";
	}
}

std::pair<std::string, std::vector<std::string>> InputHandler::GetCommandString() 
{
	std::vector<std::string> args;
	int currentArg = 0;

	COORD p = GetInputPosition();
	printf("\033[%d;%dH", p.Y, p.X);

	std::string input;
	std::getline(std::cin, input);
	LowercaseString(input);

	args.push_back(std::string());
	for (int i = 0; i < input.length(); i++)
	{
		if (input[i] == ' ')
		{
			args.push_back(std::string());
			currentArg++;
		}
		else 
		{
			args[currentArg].push_back(input[i]);
		}
	}

	bool checkVal = false;

	for (int i = 0; i < inputOptions.size(); i++)
	{
		if (inputOptions[i] == args[0])
		{
			checkVal = true;
			break;
		}
	}

	if (checkVal)
	{
		return { args[0], args };
	}
	else
	{
		return { "ERROR", args };
	}
}

void InputHandler::SetInputOptions(std::vector<std::string> options)
{
	inputOptions.clear();

	for (int i = 0; i < options.size(); i++)
	{
		inputOptions.push_back(options[i]);
	}
}