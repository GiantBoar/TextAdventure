#include "../Headers/Game.h"
#include "../Headers/ResourcePath.h"

std::string ResourcePath(std::string name) { return resourceDirectory + name; }

int main()
{
    HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);

    DWORD previousMode;
    GetConsoleMode(inputHandle, &previousMode);
    SetConsoleMode(inputHandle, ENABLE_EXTENDED_FLAGS | (previousMode & ~ENABLE_QUICK_EDIT_MODE));

    SetupInputs();

    //graphics->WriteLine("Your horse grows sluggish and heaving, dragging its sodden hooves through the thick mud.\n");
    //graphics->WriteLine("The warm glow of a campfire beams through the mist ahead of you\n");

    ChangeState(GameState::Title);

    graphics->Draw();

    while (gameRunning)
    {
        GameLoop();
    }

    SetConsoleMode(inputHandle, ENABLE_EXTENDED_FLAGS | (previousMode));
}