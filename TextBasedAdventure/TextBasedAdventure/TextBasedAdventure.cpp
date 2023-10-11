#include <iostream>
#include <stdio.h>
#include <Windows.h>

#include "Graphics.h"
#include "Input.h"

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

        //changed = ui->Changed();

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

    Sprite* title = graphics->LoadSprite("Title", "Title.txt", 1, 1, 1);
    title->position = ScreenCoord((graphics->windowSize.x - title->Width()) / 2, 4);

    graphics->LoadSprite("Background", "Sky.txt", 0, 1, 1)->SetColours(COLOUR_BRIGHT(COLOUR_BLACK));

    graphics->OrganiseButtons(graphics->GetWindowCentre() + ScreenCoord(0, 5), ScreenCoord(0, 2), std::vector<UI::Button> {
        UI::Button(ScreenCoord(1, 1), " - Start - ", true, StartGame), 
        UI::Button(ScreenCoord(1,1), " - Credits - ", true, Credits), 
        UI::Button(ScreenCoord(1,1), " - Exit - ", true, EndGame)});

    graphics->SortSprites();
}

void StartGame()
{

}

void Credits()
{
    graphics->Reset();

    Sprite* credits = graphics->LoadSprite("Credits", "Credits.txt", 1, 1, 1);
    credits->position = graphics->GetWindowCentre() - (ScreenCoord(credits->Width(), credits->spriteLines.size()) / 2);

    graphics->OrganiseButtons(ScreenCoord(graphics->GetWindowCentre().x, 25), ScreenCoord(0, 0), std::vector<UI::Button>{
        UI::Button(ScreenCoord(1, 1), " - Exit - ", true, TitleScreen)
    });
}

void EndGame()
{
    runGame = false;
}
