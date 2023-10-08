#include "Input.h"

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

void InputHandler::ProcessInput()
{
	ReadConsoleInput(inputHandle, &inputRecord, 1, &InputsRead);
	WORD currentInput = inputRecord.Event.KeyEvent.wVirtualKeyCode;

	if (keymap.find(currentInput) != keymap.end() && GetKeyState(currentInput) & KEY_PRESSED)
	{
		((void(*)())keymap[currentInput])();
	}
}


void InputHandler::AddKey(WORD key, void *data)
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