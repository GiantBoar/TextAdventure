#include <iostream>
#include <stdio.h>
#include <Windows.h>

#include "Graphics.h"
#include "Input.h"

Graphics* graphics;
InputHandler* inputs;
UI* ui;

void TitleScreen(); 
void StartGame();
void Credits();
void EndGame();

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
    //ui->DrawUI();

    bool changed = false;

    while (runGame)
    {
        //inputs->ProcessInput();

        //changed = ui->Changed();

        //if (changed) graphics->Redraw();
    }

    SetConsoleMode(inputHandle, ENABLE_EXTENDED_FLAGS | (previousMode));
}

void TitleScreen()
{
    graphics->Reset();
    ui->Reset();

    graphics->LoadSprite("Title", "Title.txt", 1, 1, 2);
    graphics->sprites[graphics->sprites.size() - 1].position.X = (SHORT)((graphics->windowSize.X - graphics->sprites[graphics->sprites.size() - 1].spriteLines[0].length()) / 2);
    graphics->LoadSprite("Background", "Sky.txt", 0, 1, 1);

    graphics->GetSprite("Background")->SetColours(COLOUR_BRIGHT(COLOUR_BLACK));

    //ui->AddElement(UI::Button(1, 1, "Start", StartGame));

    graphics->SortSprites();
}

void StartGame()
{

}

void Credits()
{

}

void EndGame()
{

}