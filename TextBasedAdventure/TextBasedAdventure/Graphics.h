#pragma once

#include <iostream>
#include <windows.h>
#include <fstream>
#include <vector>
#include <string>

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

class Graphics
{
private:
	// our singleton instance pointer
	static Graphics* _instance;

	// resize the window and initialize the class
	void Init(int width, int height);

	// private constructor for the singleton
	Graphics();

public:
	COORD windowSize;

	// our singleton instance retriever
	static Graphics* GetInstance();

	// making sure that any other attempts to create a graphics object fail
	Graphics(Graphics& other) = delete;

	// making sure that our singleton cannot be assigned to
	void operator = (const Graphics&) = delete;


	// graphics-related functions
	void Draw();
	void DrawTitle();
	void Clear();

	void ReadFromFile(const char* fileName, std::vector<std::string>& lines);
};