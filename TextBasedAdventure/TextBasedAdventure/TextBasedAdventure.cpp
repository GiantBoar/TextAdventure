#include <iostream>
#include <list>

#include "Source.h"

int main()
{
    Graphics* graphics = Graphics::GetInstance();

    //graphics->Draw();

    graphics->DrawTitle();

    while (true) {}
}