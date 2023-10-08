#include <iostream>
#include <stdio.h>
#include <Windows.h>

#include "Graphics.h"
#include "Input.h"

int main()
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);

    DWORD prev_mode;
    GetConsoleMode(hInput, &prev_mode);
    //SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS | (prev_mode & ~ENABLE_QUICK_EDIT_MODE));

    Graphics* graphics = Graphics::GetInstance();

    graphics->Draw();

    UI::Button start = UI::Button(graphics->windowSize.X / 2, graphics->windowSize.Y / 2, " - Start Game - ", nullptr);
    start.centreAligned = true;

    InputHandler* handler = InputHandler::GetInstance();

    while (true) 
    {
        //Sleep(100);

        //graphics->Redraw();

        //start.Draw();

        //handler->ProcessInput();
    }

    SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS | (prev_mode));
}

void TitleScreen()
{

}