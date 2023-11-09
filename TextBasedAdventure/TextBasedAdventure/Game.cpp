#include "Game.h"

GraphicsHandler* graphics = GraphicsHandler::GetInstance();
InputHandler* inputs = InputHandler::GetInstance();

bool gameRunning = true;

#pragma region General Game Functions

void GameLoop()
{
    inputs->ProcessInput();

    graphics->CheckAnimations();
    graphics->CheckRedraw();
}

void SetupInputs()
{
    inputs->AddKey(VK_LEFT, UI::NavigateUI);
    inputs->AddKey(VK_RIGHT, UI::NavigateUI);
    inputs->AddKey(VK_UP, UI::NavigateUI);
    inputs->AddKey(VK_DOWN, UI::NavigateUI);

    inputs->AddKey(VK_RETURN, UI::InteractUI);
}

#pragma endregion

#pragma region Menu Functions

void TitleScreen()
{
    graphics->Reset();

    Sprite* title = graphics->LoadSprite("Title", "Title.txt", 1, ScreenCoord(1, 1));
    title->position = ScreenCoord((graphics->windowSize.X - title->length) / 2, 4);
    title->SetColours(COLOUR_BRIGHT(COLOUR_CYAN));

    Sprite* bg = graphics->LoadAnimation("Background", "Mountain.txt", 0, ScreenCoord(1, 1));
    bg->SetColours(COLOUR_BRIGHT(COLOUR_BLACK));

    graphics->OrganiseButtons(graphics->GetWindowCentre() + ScreenCoord(0, 7), ScreenCoord(0, 2), std::vector<UI::Button> {
        UI::Button(ScreenCoord(1, 1), " - Start - ", true, StartGame),
            UI::Button(ScreenCoord(1, 1), " - Credits - ", true, Credits),
            UI::Button(ScreenCoord(1, 1), " - Exit - ", true, EndGame)
    });

    graphics->SortSprites();
}

void Credits()
{
    graphics->Reset();

    Sprite* credits = graphics->LoadSprite("Credits", "Credits.txt", 1, ScreenCoord(1, 1));
    credits->position = graphics->GetWindowCentre() - (ScreenCoord(credits->length, credits->height) / 2);

    graphics->OrganiseButtons(ScreenCoord(credits->position.X, 22), ScreenCoord(0, 0), std::vector<UI::Button>{
        UI::Button(ScreenCoord(1, 1), " - Exit - ", false, TitleScreen)
    });
}

void StartGame() {
    graphics->Reset();
}

void EndGame() {
    gameRunning = false;
}

#pragma endregion


std::clock_t GetTime()
{
    return std::clock();
}