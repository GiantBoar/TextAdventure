#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <windows.h>

// some definitions of colour codes I use so that I dont have to remember them or take up space in memory
#pragma region Colour Definitions
#define COLOUR_DEFAULT 0
#define COLOUR_BOLD 1
#define COLOUR_UNDERLINE 4
#define COLOUR_REVERSE 7

#define COLOUR_BRIGHT(colourcode) (colourcode + 60)
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

std::clock_t GetTime();

// structure to store an X and Y position
struct ScreenCoord : COORD
{
	ScreenCoord() {};
	ScreenCoord(short X, short Y){
		this->X = X;
		this->Y = Y;
	}

	ScreenCoord& operator-(const ScreenCoord& other){
		this->X -= other.X;
		this->Y -= other.Y;
		return *this;
	}
	ScreenCoord& operator+(const ScreenCoord& other){
		this->X += other.X;
		this->Y += other.Y;
		return *this;
	}

	ScreenCoord& operator*(const int other){
		this->X *= other;
		this->Y *= other;
		return *this;
	}
	ScreenCoord& operator/(const int other){
		this->X /= other;
		this->Y /= other;
		return *this;
	}
};

struct Sprite
{
	std::vector<std::string> spriteLines;

	const char* name;
	short sortPriority = 0;
	short colour = 0;

	short length = 0, height = 0;

	ScreenCoord position;

	Sprite(const char* name, int priority, ScreenCoord position);

	void SetColours(int colour);

	virtual void ReadFromFile(const char* fileName);
	virtual void Draw();
};

struct AnimatedSprite : Sprite
{
	typedef std::vector<std::string> Frame;

	struct Animation
	{
		unsigned int speed = 500;
		std::vector<Frame> frames;
		unsigned int lastUpdate = 0;
	};

	std::unordered_map<std::string, Animation> animations;
	std::string currentAnimation = "MAIN";
	unsigned short currentFrame = 0;

	AnimatedSprite(const char* name, int priority, ScreenCoord position);

	void PlayAnimation(std::string animation);
	void Draw() override;

	void ReadFromFile(const char* fileName) override;
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
	std::vector<Sprite*> sprites;

	std::vector<UI::Button> buttons;
	int selectedButtonIndex;

	ScreenCoord windowSize;
	bool changed = false;

	std::vector<std::string> textCache;

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
	void CheckAnimations();

	Sprite* GetSprite(const char* name);

	Sprite* LoadSprite(const char* name, const char* fileName, int priority, ScreenCoord position);
	Sprite* LoadAnimation(const char* name, const char* fileName, int priority, ScreenCoord position);
	void SortSprites();

	ScreenCoord GetWindowCentre();

	void OrganiseButtons(ScreenCoord position, ScreenCoord spacing, std::vector<UI::Button> buttons);
	void ChangeSelection(int selection);
	void Interact();

	void DrawInputBox();
};