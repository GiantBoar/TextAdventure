#include "../Headers/Graphics.h"


#pragma region Sprites

// sprite constructor
Sprite::Sprite(std::string name, int priority, ScreenCoord position, bool centreX, bool centreY)
{
	this->name = name;
	this->position = position;
	sortPriority = priority;

	this->centreX = centreX;
	this->centreY = centreY;
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
void Sprite::ReadFromFile(std::string fileName)
{
	spriteLines = std::vector<std::string>();

	std::ifstream file(ResourcePath("Sprites/" + fileName));
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

	if (centreX) position.X -= spriteLines[0].length() / 2;
	if (centreY) position.Y -= height / 2;
}

// update the colour code of the sprite
Sprite* Sprite::SetColours(int colour) 
{ 
	this->colour = colour; 
	return this; 
}


AnimatedSprite::AnimatedSprite(std::string name, int priority, ScreenCoord position, bool centreX, bool centreY) : Sprite(name, priority, position, centreX, centreY)
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

void AnimatedSprite::ReadFromFile(std::string fileName)
{
	animations = std::unordered_map<std::string, Animation>();

	std::ifstream file(ResourcePath("Sprites/" + fileName));
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

	if (centreX) position.X -= animations[animName].frames[0][0].length() / 2;
	if (centreY) position.Y -= height / 2;
}

void AnimatedSprite::PlayAnimation(std::string name)
{
	currentFrame = 0;
	if (animations.find(name) != animations.end()) currentAnimation = name;
	GraphicsHandler::GetInstance()->changed = true;
}

#pragma endregion


#pragma region GraphicsHandler

// singleton instancing 
GraphicsHandler* GraphicsHandler::_instance = nullptr;
GraphicsHandler* GraphicsHandler::GetInstance()
{
	if (_instance == NULL)
	{
		_instance = new GraphicsHandler();
	}

	return _instance;
}

// constructor for the graphics handler that functions as a way of setting up the graphics initially, such as changing the window properties and size
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

// sorting function to ensure sprites render in priority order
bool SortByPriority(Sprite* a, Sprite* b)
{
	return (a->sortPriority < b->sortPriority);
}
void GraphicsHandler::SortSprites()
{
	std::sort(sprites.begin(), sprites.end(), SortByPriority);
}

// loads a sprite from a file and allocates to the heap
Sprite* GraphicsHandler::LoadSprite(std::string name, std::string fileName, int priority, ScreenCoord position, bool centreX, bool centreY)
{
	Sprite* sprite = new Sprite(name.c_str(), priority, position, centreX, centreY);
	sprite->ReadFromFile(fileName);

	sprites.push_back(sprite);
	return (sprites[sprites.size() - 1]);
}

// loads an animation from a file and allocates it to the heap
Sprite* GraphicsHandler::LoadAnimation(std::string name, std::string fileName, int priority, ScreenCoord position, bool centreX, bool centreY)
{
	AnimatedSprite* sprite = new AnimatedSprite(name, priority, position, centreX, centreY);
	sprite->ReadFromFile(fileName);

	sprites.push_back(sprite);
	return (sprites[sprites.size() - 1]);
}

// get a pointer to the sprite with the parameter name
Sprite* GraphicsHandler::GetSprite(std::string name)
{
	for (int i = 0; i < sprites.size(); i++)
		if (sprites[i]->name == name) return sprites[i];
	return nullptr;
}

// update graphics if any animations have changed
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

void GraphicsHandler::WarnInputError()
{
	Redraw();

	printf("\033[%dmINPUT ERROR\033[0m", COLOUR_BRIGHT(COLOUR_RED));

	// wait until the pause time has passed
	Sleep(1000);

	Redraw();
}

// draws the graphics depending on the current graphics state
// if its a menu it draws all buttons and sprites using their draw() functions
// if its a text scene, it draws the scene image, the text input box, and allows for the text being typewritten to the screen
void GraphicsHandler::Draw()
{
	switch (state)
	{
	case GraphicsState::MENU:
		for (int i = 0; i < sprites.size(); i++)
		{
			sprites[i]->Draw();
		}

		DrawUI();

		printf("\033[%d;%dH", windowSize.Y, windowSize.X);

		break;

	case GraphicsState::TEXT:
		for (int i = 0; i < sprites.size(); i++)
		{
			sprites[i]->Draw();
		}

		DrawUI();

		DrawWordCache();

		DrawInputBox();

		break;
	case GraphicsState::INPUT:
		for (int i = 0; i < sprites.size(); i++)
		{
			sprites[i]->Draw();
		}

		DrawUI();

		DrawInputBox();

		break;
	}
}

void GraphicsHandler::DrawUI()
{
	for (int i = 0; i < buttons.size(); i++)
	{
		buttons[i].Draw();
	}
	for (int i = 0; i < labels.size(); i++)
	{
		labels[i].Draw();
	}
}

// simple function that clears the screen then draws it again
void GraphicsHandler::Redraw()
{
	ClearScreen();
	Draw();
}
// function that redraws the screen if the graphics have changed, and returns if the screen was redrawn
bool GraphicsHandler::CheckRedraw()
{
	if (changed)
	{
		changed = false;
		Redraw();
		return true;
	}
	return false;
}

// resets the graphics, such as deallocating the sprites and animations, and clearing the vectors
void GraphicsHandler::Reset()
{
	for (int i = 0; i < sprites.size(); i++)
	{
		delete sprites[i];
	}

	ClearWordCache();

	sprites.clear();
	buttons.clear();
	labels.clear();

	selectedButtonIndex = 0;

	changed = true;
}

// simple clearscreen function that uses the 'cls' console command to clear the console
void GraphicsHandler::ClearScreen() { system("cls"); }

// draws the input box at the bottom of the screen
void GraphicsHandler::DrawInputBox()
{
	printf("\033[%d;%dH%s", windowSize.Y - 3, 1, std::string(windowSize.X, '_').c_str());
	printf("\033[%d;%dH  > ", windowSize.Y - 1, 1);
}

// typewrites out the inputted line and adds it to the cache
void GraphicsHandler::WriteLine(std::string line)
{
	lastCacheIndex = (lastCacheIndex + 1) % WORDCACHESIZE;
	textCache[lastCacheIndex] = "";
	Redraw();

	std::string currentWord = "";

	DrawWordCache();

	for (int i = 0; i < line.length(); i++)
	{
		if (textCache[lastCacheIndex].length() + currentWord.length() >= windowSize.X - 2)
		{
			lastCacheIndex = (lastCacheIndex + 1) % WORDCACHESIZE;
			textCache[lastCacheIndex] = "";
			Redraw();
			DrawWordCache();
			std::cout << currentWord;
		}

		if (line[i] == '@')
		{
			std::string pauseArg = "";

			i++;
			while (i < line.length() && line[i] != ' ')
			{
				pauseArg.push_back(line[i]);
				if (!std::isdigit(line[i])) break;
				i++;
			}

			if(pauseArg != "") Sleep(std::stoi(pauseArg));
			continue;
		}

		currentWord.push_back(line[i]);

		if (line[i] == ' ')
		{
			textCache[lastCacheIndex] += currentWord;
			currentWord = "";
		}

		std::cout << line[i];
		Sleep(50);
	}

	if(currentWord.length() != 0) textCache[lastCacheIndex] += currentWord;
}

void GraphicsHandler::WriteLines(std::string lines[], int lineNum, int linePause)
{
	for (int i = 0; i < lineNum; i++)
	{
		WriteLine(lines[i]);

		Sleep(linePause);
	}
}

void GraphicsHandler::DrawWordCache(int yOffset)
{
	for (int i = 0; i < WORDCACHESIZE; i++)
	{
		printf("\033[%d;%dH%s", windowSize.Y - (3 + WORDCACHESIZE) + i + yOffset, 2, textCache[(i + 1 + lastCacheIndex) % WORDCACHESIZE].c_str());
	}
}

void GraphicsHandler::ClearWordCache()
{
	for (int i = 0; i < WORDCACHESIZE; i++)
	{
		textCache[i] = "";
	}
}

// concatinates a colour value code of the desired colour to the string and one to return the console to its default colour at the end
std::string GraphicsHandler::ColourString(std::string str, int colour) { return "\033[" + std::to_string(colour) + "m" + str + "\033[0m"; }

#pragma endregion


#pragma region UI

// 
// CHANGE THIS -> since it can be the only buttons that exist it should be Make Buttons or something, and should have horizontal alignment
//

void GraphicsHandler::AddLabel(UI::Label label)
{
	labels.push_back(label);
}

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

// changes the selected button by the selection offset parameter
void GraphicsHandler::ChangeSelection(int selection)
{
	if (buttons.size() == 0) return;

	buttons[selectedButtonIndex].selected = false;
	selectedButtonIndex += selection;

	selectedButtonIndex = (int)((selectedButtonIndex + buttons.size()) % buttons.size());

	buttons[selectedButtonIndex].selected = true;

	changed = true;
}

// calls the function associated with the selected button
void GraphicsHandler::Interact()
{
	if (buttons.size() == 0 || selectedButtonIndex < 0) return;

	if (buttons[selectedButtonIndex].onInteract != nullptr)
		((void(*)())buttons[selectedButtonIndex].onInteract)();
	else
		ChangeState(buttons[selectedButtonIndex].newState);
}

// namespace to keep UI functions and class functions contained
namespace UI
{
	Label::Label(std::string labelText, ScreenCoord pos, bool centred)
	{
		text = labelText;
		position = pos;
		centreAligned = centred;
	}

	void Label::Draw()
	{
		printf("\033[%d;%dH%s", position.Y, centreAligned ? position.X - (int)(text.length() / 2) : position.X, text.c_str());
	}

	// constructor for a button that assigns all its data
	Button::Button(ScreenCoord pos, std::string label, bool centred, void* interactFunction)
	{
		position = pos;

		buttonLabel = label;

		centreAligned = centred;

		onInteract = interactFunction;
	}
	
	Button::Button(ScreenCoord pos, std::string label, bool centred, GameState state)
	{
		position = pos;

		buttonLabel = label;

		centreAligned = centred;

		newState = state;
	}

	// draw function for the button, including the coloured background for a selected button
	void Button::Draw()
	{
		printf("\033[%d;%dH\033[%d;%dm%s\033[0m", position.Y, centreAligned ? position.X - (int)(buttonLabel.length() / 2) : position.X, selected ? COLOUR_BLACK : COLOUR_WHITE, selected ? COLOUR_BACKGROUND(COLOUR_WHITE) : COLOUR_BACKGROUND(COLOUR_BLACK), buttonLabel.c_str());
	}

	// navigates the UI using the user's input, assigned in the setup inputs function
	void NavigateUI(WORD word)
	{
		int input = (word == VK_DOWN || word == VK_RIGHT) ? 1 : -1;
		GraphicsHandler::GetInstance()->ChangeSelection(input);
	}

	// calls the interact function in the graphics handler if the interact button is pressed
	void InteractUI(WORD key)
	{
		GraphicsHandler::GetInstance()->Interact();
	}
}

#pragma endregion