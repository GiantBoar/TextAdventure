//#define JSON_IS_AMALGAMATION

/*
#include "json.h"
#include "json-forwards.h"
*/

#include "Game.h"

void WriteLine(std::string line, int pause)
{
    for (int i = 0; i < line.length(); i++)
    {
        std::cout << line[i];
        Sleep(30);
    }
    Sleep(pause);
}

int main()
{
    HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);

    DWORD previousMode;
    GetConsoleMode(inputHandle, &previousMode);
    SetConsoleMode(inputHandle, ENABLE_EXTENDED_FLAGS | (previousMode & ~ENABLE_QUICK_EDIT_MODE));

    SetupInputs();

    //graphics->DrawInputBox();

    graphics->WriteLine(GraphicsHandler::ColourString("fart fart fart", COLOUR_BRIGHT(COLOUR_BLUE)));

    while (true) {}

    //WriteLine("Your horse grows sluggish and heaving, dragging its sodden hooves through the thick mud.\n", 800);
    //WriteLine("The warm glow of a campfire beams through the mist ahead of you\n", 800);


    //TitleScreen();

    //graphics->Draw();

    /*
    while (gameRunning)
    {
        GameLoop();
    }*/

    SetConsoleMode(inputHandle, ENABLE_EXTENDED_FLAGS | (previousMode));
}