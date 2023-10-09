#pragma once

#include <unordered_map>
#include <iostream>
#include <Windows.h>
#include <stdio.h>

#define KEY_PRESSED 0x8000

class InputHandler
{
private:
	static InputHandler* _instance;

	INPUT_RECORD inputRecord = INPUT_RECORD();
	HANDLE inputHandle;

	DWORD InputsRead = 0;

	std::unordered_map<WORD, void(*)(WORD)> keymap;

	InputHandler();

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
};