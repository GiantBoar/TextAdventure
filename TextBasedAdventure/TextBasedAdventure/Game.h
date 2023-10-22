#pragma once

#include <chrono>
#include <iostream>
#include <windows.h>

#include "Graphics.h"
#include "Input.h"
extern class GraphicsHandler* graphics;
extern class InputHandler* inputs;

extern bool gameRunning;

void GameLoop();

void TitleScreen();
void Credits();
void StartGame();
void EndGame();

void SetupInputs();

std::clock_t GetTime();