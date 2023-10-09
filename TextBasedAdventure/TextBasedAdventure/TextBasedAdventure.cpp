#include <iostream>
#include <stdio.h>
#include <Windows.h>

#include "Graphics.h"
#include "Input.h"

Graphics* graphics;
InputHandler* inputs;
UI* ui;

void TitleScreen();

bool runGame = true;

int main()
{
    HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);

    DWORD previousMode;
    GetConsoleMode(inputHandle, &previousMode);
    SetConsoleMode(inputHandle, ENABLE_EXTENDED_FLAGS | (previousMode & ~ENABLE_QUICK_EDIT_MODE));

    graphics = Graphics::GetInstance();
    inputs = InputHandler::GetInstance();
    ui = UI::GetInstance();

    TitleScreen();

    graphics->Draw();

    bool changed = false;

    while (runGame)
    {
        inputs->ProcessInput();


        changed = ui->Changed();

        if (changed) graphics->Redraw();
    }

    SetConsoleMode(inputHandle, ENABLE_EXTENDED_FLAGS | (previousMode));
}

void TitleScreen()
{
    graphics->Reset();

    graphics->LoadSprite("Title.txt", 1, 1, 2);
    graphics->sprites[graphics->sprites.size() - 1].position.X = (graphics->windowSize.X - graphics->sprites[graphics->sprites.size() - 1].spriteLines[0].length()) / 2;
    graphics->LoadSprite("Sky.txt", 0, 1, 1);

    graphics->SortSprites();
}