#include "Graphics.h"

#pragma region GraphicsHandler

GraphicsHandler* GraphicsHandler::_instance = nullptr;

GraphicsHandler* GraphicsHandler::GetInstance()
{
	if (_instance == NULL)
	{
		_instance = new GraphicsHandler();
	}

	return _instance;
}

void GraphicsHandler::Init(short width, short height)
{
	CONSOLE_SCREEN_BUFFER_INFOEX consoleScreenBuffer = CONSOLE_SCREEN_BUFFER_INFOEX();
	consoleScreenBuffer.cbSize = sizeof(consoleScreenBuffer);

	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfoEx(consoleHandle, &consoleScreenBuffer);

	windowSize = ScreenCoord(width, height);

	consoleScreenBuffer.dwSize.X = windowSize.x;
	consoleScreenBuffer.dwSize.Y = windowSize.y;

	consoleScreenBuffer.srWindow.Left = 0;
	consoleScreenBuffer.srWindow.Right = windowSize.y;
	consoleScreenBuffer.srWindow.Top = 0;
	consoleScreenBuffer.srWindow.Bottom = windowSize.x;

	SetConsoleScreenBufferInfoEx(consoleHandle, &consoleScreenBuffer);

	selectedButtonIndex = 0;
}

GraphicsHandler::GraphicsHandler()
{
	Init(120, 30);
}

ScreenCoord GraphicsHandler::GetWindowCentre() { return ScreenCoord(windowSize.x / 2, windowSize.y / 2); }

bool SortByPriority(Sprite& a, Sprite& b)
{
	return (a.sortPriority < b.sortPriority);
}
void GraphicsHandler::SortSprites()
{
	std::sort(sprites.begin(), sprites.end(), SortByPriority);
}

Sprite* GraphicsHandler::LoadSprite(const char* name, const char* fileName, int priority, int x, int y)
{
	Sprite sprite = Sprite(name, fileName, priority, x, y);
	sprites.push_back(sprite);
	return &(sprites[sprites.size() - 1]);
}

Sprite* GraphicsHandler::GetSprite(const char* name)
{
	for (int i = 0; i < sprites.size(); i++)
	{
		if (sprites[i].name == name) return &sprites[i];
	}
	return nullptr;
}


void GraphicsHandler::Draw()
{
	for (int i = 0; i < sprites.size(); i++)
	{
		sprites[i].Draw();
	}

	for (int i = 0; i < buttons.size(); i++)
	{
		buttons[i].Draw();
	}

	printf("\033[%d;%dH", windowSize.y, windowSize.x);
}

void GraphicsHandler::Redraw()
{
	ClearScreen();
	Draw();
}

void GraphicsHandler::Reset()
{
	sprites.clear();
	buttons.clear();

	selectedButtonIndex = 0;

	changed = true;
}

void GraphicsHandler::ClearScreen() { system("cls"); }

bool GraphicsHandler::Changed()
{
	if (changed)
	{
		changed = false;
		return true;
	}
	return false;
}

#pragma endregion


#pragma region Sprites

Sprite::Sprite(const char* name, const char* fileName, int priority, int x, int y)
{
	this->name = name;

	position = ScreenCoord(x, y);

	sortPriority = priority;

	ReadFromFile(fileName);
}

void Sprite::Draw()
{
	for (int i = 0; i < spriteLines.size(); i++)
	{
		printf("\033[%d;%dH\033[%dm%s\033[0m", position.y + i, position.x, colour, spriteLines[i].c_str());
	}
}

void Sprite::ReadFromFile(const char* fileName)
{
	spriteLines = std::vector<std::string>();
	spriteLines.clear();

	std::ifstream file(fileName);
	std::string s;

	while (getline(file, s))
	{
		spriteLines.push_back(s);
	}
}

void Sprite::SetColours(int colour)
{
	this->colour = colour;
}

int Sprite::Width() { return (int)(spriteLines[0].length()); }

#pragma endregion


#pragma region UI

void GraphicsHandler::OrganiseButtons(ScreenCoord position, ScreenCoord spacing, std::vector<UI::Button> buttons)
{
	if (this->buttons.size() != 0) return; // shouldn't organise a new array of buttons if buttons already exist, cause it will mess with the selection and interaction system

	selectedButtonIndex = 0;

	for (int i = 0; i < buttons.size(); i++)
	{
		buttons[i].position = position;

		buttons[i].position.x += spacing.x * i;
		buttons[i].position.y += spacing.y * i;

		this->buttons.push_back(buttons[i]);
	}

	this->buttons[selectedButtonIndex].selected = true;
}

void GraphicsHandler::ChangeSelection(int selection)
{
	if (buttons.size() == 0) return;

	buttons[selectedButtonIndex].selected = false;
	selectedButtonIndex += selection;

	selectedButtonIndex = (selectedButtonIndex + buttons.size()) % buttons.size();

	buttons[selectedButtonIndex].selected = true;

	changed = true;
}

void GraphicsHandler::Interact()
{
	if (buttons.size() == 0 || selectedButtonIndex < 0) return;

	((void(*)())buttons[selectedButtonIndex].onInteract)();
}

namespace UI
{
	Button::Button(ScreenCoord pos, std::string label, bool centred, void* interactFunction)
	{
		position = pos;

		buttonLabel = label;

		centreAligned = centred;

		onInteract = interactFunction;
	}

	void Button::Draw()
	{
		printf("\033[%d;%dH\033[%d;%dm%s\033[0m", position.y, centreAligned ? position.x - (int)(buttonLabel.length() / 2) : position.x, selected ? COLOUR_BLACK : COLOUR_WHITE, selected ? COLOUR_BACKGROUND(COLOUR_WHITE) : COLOUR_BACKGROUND(COLOUR_BLACK), buttonLabel.c_str());
	}

	void NavigateUI(WORD word)
	{

		/*
		ScreenCoord input;
		input.x = (word == VK_LEFT || word == VK_RIGHT) ? ((word == VK_LEFT) ? -1 : 1) : 0;
		input.y = (word == VK_UP || word == VK_DOWN) ? ((word == VK_UP) ? 1 : -1) : 0;
		*/

		int input = (word == VK_DOWN || word == VK_RIGHT) ? 1 : -1;
		GraphicsHandler::GetInstance()->ChangeSelection(input);
	}

	void InteractUI(WORD key)
	{
		GraphicsHandler::GetInstance()->Interact();
	}
}

#pragma endregion
