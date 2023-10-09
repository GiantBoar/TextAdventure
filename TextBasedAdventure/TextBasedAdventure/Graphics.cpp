#include "Graphics.h"

Graphics* Graphics::_instance = nullptr;

Graphics* Graphics::GetInstance()
{
	if (_instance == NULL)
	{
		_instance = new Graphics();
	}

	return _instance;
}

void Graphics::Init(short width, short height)
{
	CONSOLE_SCREEN_BUFFER_INFOEX consoleScreenBuffer;
	consoleScreenBuffer.cbSize = sizeof(consoleScreenBuffer);

	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfoEx(consoleHandle, &consoleScreenBuffer);

	windowSize.X = width;
	windowSize.Y = height;
	consoleScreenBuffer.dwSize = windowSize;

	consoleScreenBuffer.srWindow.Left = 0;
	consoleScreenBuffer.srWindow.Right = windowSize.X;
	consoleScreenBuffer.srWindow.Top = 0;
	consoleScreenBuffer.srWindow.Bottom = windowSize.Y;

	SetConsoleScreenBufferInfoEx(consoleHandle, &consoleScreenBuffer);
}

Graphics::Graphics()
{
	sprites.clear();

	Init(120, 30);
}

bool SortByPriority(Sprite& a, Sprite& b)
{
	return (a.sortPriority < b.sortPriority);
}
void Graphics::SortSprites()
{
	std::sort(sprites.begin(), sprites.end(), SortByPriority);
}

void Graphics::Draw()
{
	for (int i = 0; i < sprites.size(); i++)
	{
		sprites[i].Draw();
	}
}

void Graphics::Redraw()
{
	Clear();
	Draw();
}

void Graphics::Clear()
{
	system("cls");
}

void Graphics::Reset()
{

}

void Graphics::LoadSprite(const char* fileName, int priority, int x, int y)
{
	Sprite sprite = Sprite(fileName, priority, x, y);
	sprites.push_back(sprite);
}




Sprite::Sprite(const char* fileName, int priority, int x, int y)
{
	position.X = x;
	position.Y = y;

	sortPriority = priority;

	ReadFromFile(fileName);
}

void Sprite::Draw()
{
	for (int i = 0; i < spriteLines.size(); i++)
	{
		printf("\033[%d;%dH%s", position.Y + i, position.X, spriteLines[i].c_str());
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