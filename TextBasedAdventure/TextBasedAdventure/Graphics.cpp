#include "Graphics.h"


#pragma region Sprites

// sprite constructor
Sprite::Sprite(const char* name, int priority, ScreenCoord position)
{
	this->name = name;
	this->position = position;
	sortPriority = priority;
}

// draw the sprite
void Sprite::Draw()
{
	for (int i = 0; i < spriteLines.size(); i++)
	{
		printf("\033[%d;%dH\033[%dm%s\033[0m", position.Y + i, position.X, colour, spriteLines[i].c_str());
	}
}

// reads the lines from a file into a vector to be outputted later on
void Sprite::ReadFromFile(const char* fileName)
{
	spriteLines = std::vector<std::string>();

	std::ifstream file(fileName);
	std::string s;

	int lineCount = 0;
	while (getline(file, s))
	{
		if (s.length() == 0) continue;

		lineCount++;

		if (length == 0 || s.length() > length) length = (int)s.length();

		spriteLines.push_back(s);
	}

	if (height == 0 || lineCount > height) height = lineCount;
}

// update the colour code of the sprite
void Sprite::SetColours(int colour) { this->colour = colour; }


AnimatedSprite::AnimatedSprite(const char* name, int priority, ScreenCoord position) : Sprite(name, priority, position)
{
	this->name = name;
	this->position = position;
	sortPriority = priority;
}

void AnimatedSprite::Draw()
{
	for (int i = 0; i < animations[currentAnimation].frames[currentFrame].size(); i++)
	{
		printf("\033[%d;%dH\033[%dm%s\033[0m", position.Y + i, position.X, colour, animations[currentAnimation].frames[currentFrame][i].c_str());
	}
}

void AnimatedSprite::ReadFromFile(const char* fileName)
{
	animations = std::unordered_map<std::string, Animation>();

	std::ifstream file(fileName);
	std::string s, animName;
	animName = "MAIN";
	int frame = 0;

	animations[animName].frames.push_back(AnimatedSprite::Frame());

	int lineCount = 0;
	while (getline(file, s))
	{
		if (s.length() == 0) continue;

		lineCount++;

		if (s[0] == '$')
		{
			std::string name = s.substr(2, s.length());
			animName = name;
			animations[animName].frames.push_back(AnimatedSprite::Frame());
			frame = 0;

			if (height == 0 || lineCount > height) height = lineCount;
			lineCount = 0;

			continue;
		}
		else if (s[0] == '&')
		{
			frame++;
			animations[animName].frames.push_back(AnimatedSprite::Frame());
			if (height == 0 || lineCount > height) height = lineCount;
			lineCount = 0;
			continue;
		}

		if (length == 0 || s.length() > length) length = (int)s.length();

		animations[animName].frames[frame].push_back(s);
	}

	if (height == 0 || lineCount > height) height = lineCount;
}

void AnimatedSprite::PlayAnimation(std::string name)
{
	currentFrame = 0;
	if (animations.find(name) != animations.end()) currentAnimation = name;
	GraphicsHandler::GetInstance()->changed = true;
}

#pragma endregion


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

GraphicsHandler::GraphicsHandler()
{
	SetConsoleOutputCP(65001);

	CONSOLE_SCREEN_BUFFER_INFOEX consoleScreenBuffer = CONSOLE_SCREEN_BUFFER_INFOEX();
	consoleScreenBuffer.cbSize = sizeof(consoleScreenBuffer);

	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfoEx(consoleHandle, &consoleScreenBuffer);

	windowSize = ScreenCoord(120, 30);

	consoleScreenBuffer.dwSize.X = windowSize.X;
	consoleScreenBuffer.dwSize.Y = windowSize.Y;

	consoleScreenBuffer.srWindow.Left = 0;
	consoleScreenBuffer.srWindow.Right = windowSize.Y;
	consoleScreenBuffer.srWindow.Top = 0;
	consoleScreenBuffer.srWindow.Bottom = windowSize.X;

	SetConsoleScreenBufferInfoEx(consoleHandle, &consoleScreenBuffer);

	state = GraphicsState::MENU;

	selectedButtonIndex = 0;
}

ScreenCoord GraphicsHandler::GetWindowCentre() { return ScreenCoord(windowSize.X / 2, windowSize.Y / 2); }

bool SortByPriority(Sprite* a, Sprite* b)
{
	return (a->sortPriority < b->sortPriority);
}
void GraphicsHandler::SortSprites()
{
	std::sort(sprites.begin(), sprites.end(), SortByPriority);
}

Sprite* GraphicsHandler::LoadSprite(const char* name, const char* fileName, int priority, ScreenCoord position)
{
	Sprite* sprite = new Sprite(name, priority, position);
	sprite->ReadFromFile(fileName);

	sprites.push_back(sprite);
	return (sprites[sprites.size() - 1]);
}

Sprite* GraphicsHandler::LoadAnimation(const char* name, const char* fileName, int priority, ScreenCoord position)
{
	AnimatedSprite* sprite = new AnimatedSprite(name, priority, position);
	sprite->ReadFromFile(fileName);

	sprites.push_back(sprite);
	return (sprites[sprites.size() - 1]);
}

Sprite* GraphicsHandler::GetSprite(const char* name)
{
	for (int i = 0; i < sprites.size(); i++)
	{
		if (sprites[i]->name == name) return sprites[i];
	}
	return nullptr;
}

void GraphicsHandler::CheckAnimations()
{
	int t = GetTime();
	AnimatedSprite* ptr;
	for (int i = 0; i < sprites.size(); i++)
	{
		ptr = dynamic_cast<AnimatedSprite*>(sprites[i]);
		if (ptr == nullptr) continue;

		if (t - ptr->animations[ptr->currentAnimation].lastUpdate > ptr->animations[ptr->currentAnimation].speed)
		{
			ptr->currentFrame = (ptr->currentFrame + 1) % ptr->animations[ptr->currentAnimation].frames.size();
			ptr->animations[ptr->currentAnimation].lastUpdate = t;

			changed = true;
		}
	}
}


void GraphicsHandler::Draw()
{
	switch (state)
	{
	case GraphicsState::MENU:
		for (int i = 0; i < sprites.size(); i++)
		{
			sprites[i]->Draw();
		}

		for (int i = 0; i < buttons.size(); i++)
		{
			buttons[i].Draw();
		}

		printf("\033[%d;%dH", windowSize.Y, windowSize.X);

		break;

	case GraphicsState::TEXT:
		DrawInputBox();
	}
}

void GraphicsHandler::Redraw()
{
	ClearScreen();
	Draw();
}

void GraphicsHandler::Reset()
{
	for (int i = 0; i < sprites.size(); i++)
	{
		delete sprites[i];
	}

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



void GraphicsHandler::DrawInputBox()
{
	printf("\033[%d;%dH%s", windowSize.Y - 3, 1, std::string(windowSize.X, '_').c_str());
	printf("\033[%d;%dH  > ", windowSize.Y - 1, 1);
}

void GraphicsHandler::WriteLine(std::string line)
{
	Redraw();

	lastCacheIndex = (lastCacheIndex + 1) % WORDCACHESIZE;
	textCache[lastCacheIndex] = "";

	for (int i = 0; i < WORDCACHESIZE; i++)
	{
		printf("\033[%d;%dH%s", windowSize.Y - (3 + WORDCACHESIZE) + i, 2, textCache[(i + 1 + lastCacheIndex) % 6].c_str());
	}

	textCache[lastCacheIndex] = line;

	for (int i = 0; i < line.length(); i++)
	{
		std::cout << line[i];
		Sleep(30);
	}
}




std::string GraphicsHandler::ColourString(std::string str, int colour)
{
	return "\033[" + std::to_string(colour) + "m" + str + "\033[0m";
}

#pragma endregion


#pragma region UI

void GraphicsHandler::OrganiseButtons(ScreenCoord position, ScreenCoord spacing, std::vector<UI::Button> buttons)
{
	if (this->buttons.size() != 0) return; // shouldn't organise a new array of buttons if buttons already exist, cause it will mess with the selection and interaction system

	selectedButtonIndex = 0;

	for (int i = 0; i < buttons.size(); i++)
	{
		buttons[i].position = position;

		buttons[i].position.X += spacing.X * i;
		buttons[i].position.Y += spacing.Y * i;

		this->buttons.push_back(buttons[i]);
	}

	this->buttons[selectedButtonIndex].selected = true;
}

void GraphicsHandler::ChangeSelection(int selection)
{
	if (buttons.size() == 0) return;

	buttons[selectedButtonIndex].selected = false;
	selectedButtonIndex += selection;

	selectedButtonIndex = ((size_t)selectedButtonIndex + buttons.size()) % buttons.size();

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
		printf("\033[%d;%dH\033[%d;%dm%s\033[0m", position.Y, centreAligned ? position.X - (int)(buttonLabel.length() / 2) : position.X, selected ? COLOUR_BLACK : COLOUR_WHITE, selected ? COLOUR_BACKGROUND(COLOUR_WHITE) : COLOUR_BACKGROUND(COLOUR_BLACK), buttonLabel.c_str());
	}

	void NavigateUI(WORD word)
	{
		int input = (word == VK_DOWN || word == VK_RIGHT) ? 1 : -1;
		GraphicsHandler::GetInstance()->ChangeSelection(input);
	}

	void InteractUI(WORD key)
	{
		GraphicsHandler::GetInstance()->Interact();
	}
}

#pragma endregion