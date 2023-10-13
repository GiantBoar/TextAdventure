#pragma once

#include <iostream>
#include <windows.h>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

#pragma region Colour Definitions
#define COLOUR_DEFAULT 0
#define COLOUR_BOLD 1
#define COLOUR_UNDERLINE 4
#define COLOUR_REVERSE 7

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

struct ScreenCoord
{
	int x = 0, y = 0;

	ScreenCoord(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	ScreenCoord() {}

	ScreenCoord& operator-(const ScreenCoord& other)
	{
		this->x -= other.x;
		this->y -= other.y;
		return *this;
	}
	ScreenCoord& operator+(const ScreenCoord& other)
	{
		this->x += other.x;
		this->y += other.y;
		return *this;
	}

	ScreenCoord& operator*(const int other)
	{
		this->x *= other;
		this->y *= other;
		return *this;
	}
	ScreenCoord& operator/(const int other)
	{
		this->x /= other;
		this->y /= other;
		return *this;
	}
};

class Sprite 
{
public:
	typedef std::vector<std::string> Frame;

	class Animation
	{
	public:
		float speed = .5f;
		std::vector<Frame> frames;
	};

	std::unordered_map<std::string, Animation> animations;

	const char* name;

	std::string currentAnimation = "MAIN";
	int currentFrame = 0;

	int sortPriority = 0;
	int colour = 0;

	int length = 0;
	int height = 0;

	ScreenCoord position;

	Sprite(const char* name, const char* fileName, int priority, ScreenCoord position);

	void ReadFromFile(const char* fileName);
	void SetColours(int colour);

	void Draw();

	void PlayAnimation(std::string animation);
};

namespace UI
{
	class Button
	{
	public:
		ScreenCoord position;

		std::string buttonLabel;

		void* onInteract = nullptr;
		bool selected = false;

		bool centreAligned;

		Button(ScreenCoord pos, std::string label, bool centreAligned, void* interactFunction);

		void Draw();
	};

	void NavigateUI(WORD key);

	void InteractUI(WORD key);
}

class GraphicsHandler
{
private:
	// our singleton instance pointer
	static GraphicsHandler* _instance;

	// private constructor for the singleton
	GraphicsHandler();

public:
	std::vector<Sprite> sprites;

	std::vector<UI::Button> buttons;
	int selectedButtonIndex;

	ScreenCoord windowSize;
	bool changed = false;

	// our singleton instance retriever
	static GraphicsHandler* GetInstance();

	// making sure that any other attempts to create a graphics object fail
	GraphicsHandler(GraphicsHandler& other) = delete;

	// making sure that our singleton cannot be assigned to
	void operator = (const GraphicsHandler&) = delete;

	// graphics-related functions
	void Draw();
	void Redraw();
	void ClearScreen();
	void Reset();

	bool Changed();

	Sprite* GetSprite(const char* name);

	Sprite* LoadSprite(const char* name, const char* fileName, int priority, ScreenCoord position);
	void SortSprites();

	void CheckAnimations();

	ScreenCoord GetWindowCentre();

	void OrganiseButtons(ScreenCoord position, ScreenCoord spacing, std::vector<UI::Button> buttons);
	void ChangeSelection(int selection);
	void Interact();
};