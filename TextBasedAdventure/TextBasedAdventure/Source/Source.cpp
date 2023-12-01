#include "../Headers/Game.h"
#include "../Headers/ResourcePath.h"

std::string ResourcePath(std::string name) { return resourceDirectory + name; }

int main()
{
    HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);

    // fix reoccuring randomness
    srand(time(0));

    DWORD previousMode;
    GetConsoleMode(inputHandle, &previousMode);
    SetConsoleMode(inputHandle, ENABLE_EXTENDED_FLAGS | (previousMode & ~ENABLE_QUICK_EDIT_MODE));

    if (SaveSystem::CanLoadSave()) SaveSystem::LoadPlayerData(playerData);

    SetupInputs();

    ChangeState(GameState::Title);

    graphics->Draw();

    while (gameRunning)
    {
        GameLoop();
    }

    delete graphics;
    delete inputs;
    delete playerData;
    delete currentLevel;

    SetConsoleMode(inputHandle, ENABLE_EXTENDED_FLAGS | (previousMode));
}