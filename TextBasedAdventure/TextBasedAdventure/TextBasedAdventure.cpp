#include <iostream>
#include <list>

#include "Graphics.h"

int main()
{
    Graphics* graphics = Graphics::GetInstance();

    graphics->DrawTitle();

    while (true) 
    {
        Sleep(100);

        graphics->Clear();
        graphics->DrawTitle();
    }
}