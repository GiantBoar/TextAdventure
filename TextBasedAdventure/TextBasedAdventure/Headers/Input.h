#pragma once

#include <unordered_map>
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <string>

#define KEY_PRESSED 0x8000

std::clock_t GetTime();

void LowercaseString(std::string& str);
COORD GetInputPosition();

typedef std::pair<std::string, std::vector<std::string>> Command;

class InputHandler
{
private:
	static InputHandler* _instance;

	INPUT_RECORD inputRecord = INPUT_RECORD();
	HANDLE inputHandle;

	DWORD InputsRead = 0;

	std::unordered_map<WORD, void(*)(WORD)> keymap;

	InputHandler();

	bool splitInput;
	std::vector<std::string> inputOptions;

public:
	class KeyData
	{
		void* onKeyDown = nullptr;

	public:
		bool pressed = false;

		KeyData() {}
		KeyData(void* onPress);
	};

	static InputHandler* GetInstance();

	void ProcessInput();

	void AddKey(WORD key, void (*data)(WORD));
	void RemoveKey(WORD key);

	std::string GetRawInputString();
	std::string GetInputString();
	Command GetCommandString();

	void SetInputOptions(std::vector<std::string> options);
};