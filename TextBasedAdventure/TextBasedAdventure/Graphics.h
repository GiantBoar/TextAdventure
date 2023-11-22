#pragma once

#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <windows.h>
#include <string>

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

#define WORDCACHESIZE 5

enum class GameState;

std::clock_t GetTime();

void ChangeState(GameState);

// structure to store an X and Y position
struct ScreenCoord : COORD
{
	ScreenCoord() {};
	ScreenCoord(short X, short Y){
		this->X = X;
		this->Y = Y;
	}

	ScreenCoord operator-(const ScreenCoord& other){
		return ScreenCoord(this->X - other.X, this->Y - other.Y);
	}
	ScreenCoord operator+(const ScreenCoord& other){
		return ScreenCoord(this->X + other.X, this->Y + other.Y);
	}
	ScreenCoord& operator+=(const ScreenCoord& other) {
		this->X += other.X;
		this->Y += other.Y;
		return *this;
	}

	ScreenCoord operator*(const int other){
		return ScreenCoord(this->X * other, this->Y * other);
	}
	ScreenCoord operator/(const int other){
		return ScreenCoord(this->X / other, this->Y / other);
	}
};

struct Sprite
{
	std::vector<std::string> spriteLines;

	std::string name;
	short sortPriority = 0;
	short colour = 0;

	short length = 0, height = 0;

	ScreenCoord position;

	Sprite(const char* name, int priority, ScreenCoord position);

	Sprite* SetColours(int colour);

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
	class Label
	{
		ScreenCoord position;
		bool centreAligned;
		std::string text;

		Label(std::string labelText, ScreenCoord pos, bool centred);

		void Draw();
	};

	class Button
	{
	public:
		ScreenCoord position;

		std::string buttonLabel;

		GameState newState;
		void* onInteract = nullptr;
		bool selected = false;

		bool centreAligned;

		Button(ScreenCoord pos, std::string label, bool centreAligned, void* interactFunction);
		Button(ScreenCoord pos, std::string label, bool centreAligned, GameState changeState);

		void Draw();
	};

	void NavigateUI(WORD key);

	void InteractUI(WORD key);
}

enum class GraphicsState
{
	MENU,
	TEXT,
	BATTLE,
	INPUT
};

class GraphicsHandler
{
private:
	// our singleton instance pointer
	static GraphicsHandler* _instance;

	// private constructor for the singleton
	GraphicsHandler();

public:
	GraphicsState state;

	std::vector<Sprite*> sprites;

	std::vector<UI::Button> buttons;
	int selectedButtonIndex;

	ScreenCoord windowSize;
	bool changed = false;

	std::string textCache[WORDCACHESIZE];
	short lastCacheIndex = 0;

	// our singleton instance retriever
	static GraphicsHandler* GetInstance();

	// making sure that any other attempts to create a graphics object fail
	GraphicsHandler(GraphicsHandler& other) = delete;

	// making sure that our singleton cannot be assigned to
	void operator = (const GraphicsHandler&) = delete;

	// graphics-related functions
	void Draw();
	void Redraw();
	bool CheckRedraw();
	void ClearScreen();
	void Reset();

	void CheckAnimations();

	void WarnInputError();

	Sprite* GetSprite(std::string name);

	Sprite* LoadSprite(std::string name, std::string fileName, int priority, ScreenCoord position);
	Sprite* LoadAnimation(const char* name, const char* fileName, int priority, ScreenCoord position);
	void SortSprites();

	ScreenCoord GetWindowCentre();

	void OrganiseButtons(ScreenCoord position, ScreenCoord spacing, std::vector<UI::Button> buttons);
	void ChangeSelection(int selection);
	void Interact();

	void DrawInputBox();
	void WriteLine(std::string line);
	void WriteLine(std::string lines[], int lineCount);
	void DrawWordCache();
	void ClearWordCache();

	static std::string ColourString(std::string str, int colour);
};