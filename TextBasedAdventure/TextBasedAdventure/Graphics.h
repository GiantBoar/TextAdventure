#pragma once

#include <iostream>
#include <windows.h>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include "UI.h"

#pragma region Colour Definitions
#define COLOUR_DEFAULT 0
#define COLOUR_BOLD 1
#define COLOUR_UNDERLINE 4
#define COLOUR_REVERSE 7

#define COLOUR_CODE R"(\033[)"
#define COLOUR_BRIGHT(colourcode) (colourcode + 60)
#define COLOUR_FOREGROUND(colourcode) (colourcode)
#define COLOUR_BACKGROUND(colourcode) (colourcode + 10)

#define COLOUR_BLACK 30
#define COLOUR_RED 31
#define COLOUR_GREEN 32
#define COLOUR_YELLOW 33
#define COLOUR_BLUE 34
#define COLOUR_PURPLE 35
#define COLOUR_CYAN 36
#define COLOUR_WHITE 37
#define COLOUR_GREY COLOUR_BRIGHT(COLOUR_BLACK)
#pragma endregion


struct Sprite 
{
public:
	std::vector<std::string> spriteLines;

	const char* name;

	int sortPriority = 0;
	int colour = 0;

	COORD position;

	Sprite(const char* name, const char* fileName, int priority, int x, int y);

	void ReadFromFile(const char* fileName);
	void SetColours(int colour);

	void Draw();
};

class Graphics
{
private:
	// our singleton instance pointer
	static Graphics* _instance;

	// resize the window and initialize the class
	void Init(short width, short height);

	// private constructor for the singleton
	Graphics();

public:
	std::vector<Sprite> sprites;

	COORD windowSize;

	// our singleton instance retriever
	static Graphics* GetInstance();

	// making sure that any other attempts to create a graphics object fail
	Graphics(Graphics& other) = delete;

	// making sure that our singleton cannot be assigned to
	void operator = (const Graphics&) = delete;

	// graphics-related functions
	void Draw();
	void Redraw();
	void Clear();

	void Reset();

	Sprite* GetSprite(const char* name);

	Sprite* LoadSprite(const char* name, const char* fileName, int priority, int x, int y);
	void SortSprites();
};