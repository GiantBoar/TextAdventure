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

void Graphics::Init(int width, int height)
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
	Init(120, 30);
}

void Graphics::Draw()
{
	DrawTitle();
}

void Graphics::Redraw()
{
	Clear();
	Draw();
}

void Graphics::DrawTitle()
{
	short x = 0, y = 0, type = 0;

	srand(192);
	for (int i = 0; i < 50; i++)
	{
		x = rand() % windowSize.X + 1;
		y = rand() % windowSize.Y + 1;
		type = rand() % 2;

		printf("\033[%d;%dH%c", y, x, (type == 0) ? '.' : '+');
	}

	std::vector<std::string> titleCard;
	ReadFromFile("title.txt", titleCard);

	short horizontalSpacing = (windowSize.X - titleCard[0].length()) / 2;

	for (int i = 0; i < titleCard.size(); i++)
	{
		//printf("\033[%d;%dH%s", i + 3, horizontalSpacing, titleCard[i].c_str());
	}
}

void Graphics::Clear()
{
	system("cls");
}

void Graphics::ReadFromFile(const char* fileName, std::vector<std::string>& lines)
{
	lines.clear();
	std::ifstream file(fileName);
	std::string s;
	while (getline(file, s))
		lines.push_back(s);
}






/*void Graphics::Clear()
{
	system("cls");
}

void Graphics::Draw()
{
	/*
	ifstream bgfile;
	bgfile.open("Sprite.txt");

	int screenWidth = Graphics::windowSize.X;
	//std::string lineread;
	char lineread;

	ifstream file("Sprite.txt");

	for (int y = 0; y < Graphics::windowSize.Y; y++)
	{
		for (int x = 0; x < Graphics::windowSize.X; x++)
		{

		}
		cout << endl;
	}
}

Sprite::Sprite(string fileName)
{
	ifstream file(fileName);
}

void Sprite::LoadSprite(std::string fileName)
{
	ifstream file(fileName);
	string str;

	while (getline(file, str)) {
		this.sprite.push_back(str);
	}
}*/