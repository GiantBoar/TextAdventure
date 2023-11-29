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

    ChangeState(GameState::Title);

    graphics->Draw();

    while (gameRunning)
    {
        GameLoop();
    }

    SetConsoleMode(inputHandle, ENABLE_EXTENDED_FLAGS | (previousMode));
}