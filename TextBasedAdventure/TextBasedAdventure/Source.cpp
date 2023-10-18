//#define JSON_IS_AMALGAMATION

#include <iostream>
#include <stdio.h>
#include <Windows.h>

/*
#include "json.h"
#include "json-forwards.h"
*/

#include "Game.h"

GraphicsHandler* graphics;
InputHandler* inputs;

void SetupInputs();
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

    graphics = GraphicsHandler::GetInstance();
    inputs = InputHandler::GetInstance();

    SetupInputs();

    TitleScreen();

    graphics->Draw();

    bool changed = false;

    while (runGame)
    {
        inputs->ProcessInput();

        graphics->CheckAnimations();

        if (graphics->Changed()) graphics->Redraw();
    }

    SetConsoleMode(inputHandle, ENABLE_EXTENDED_FLAGS | (previousMode));
}

void SetupInputs()
{
    inputs->AddKey(VK_LEFT, UI::NavigateUI);
    inputs->AddKey(VK_RIGHT, UI::NavigateUI);
    inputs->AddKey(VK_UP, UI::NavigateUI);
    inputs->AddKey(VK_DOWN, UI::NavigateUI);

    inputs->AddKey(VK_RETURN, UI::InteractUI);
}

void TitleScreen()
{
    graphics->Reset();

    Sprite* title = graphics->LoadSprite("Title", "Title.txt", 1, ScreenCoord(1,1));
    title->position = ScreenCoord((graphics->windowSize.X - title->length) / 2, 4);
    title->SetColours(COLOUR_BRIGHT(COLOUR_CYAN));

    Sprite* bg = graphics->LoadAnimation("Background", "Mountain.txt", 0, ScreenCoord(1,1));
    bg->SetColours(COLOUR_BRIGHT(COLOUR_BLACK));

    graphics->OrganiseButtons(graphics->GetWindowCentre() + ScreenCoord(0, 7), ScreenCoord(0, 2), std::vector<UI::Button> {
        UI::Button(ScreenCoord(1, 1), " - Start - ", true, StartGame), 
        UI::Button(ScreenCoord(1,1), " - Credits - ", true, Credits), 
        UI::Button(ScreenCoord(1,1), " - Exit - ", true, EndGame)
    });

    graphics->SortSprites();
}

void StartGame()
{
    graphics->Reset();
}

void Credits()
{
    graphics->Reset();

    Sprite* credits = graphics->LoadSprite("Credits", "Credits.txt", 1, ScreenCoord(1,1));
    credits->position = graphics->GetWindowCentre() - (ScreenCoord(credits->length, credits->height) / 2);

    graphics->OrganiseButtons(ScreenCoord(credits->position.X, 22), ScreenCoord(0, 0), std::vector<UI::Button>{
        UI::Button(ScreenCoord(1, 1), " - Exit - ", false, TitleScreen)
    }); 
}

void EndGame()
{
    runGame = false;
}
