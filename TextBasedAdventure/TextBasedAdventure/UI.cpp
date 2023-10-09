#include "UI.h"
#include "Graphics.h"
#include "Input.h"

#pragma region Main UI

UI* UI::_instance = nullptr;

UI* UI::GetInstance()
{
	if (_instance == NULL)
	{
		_instance = new UI();
	}

	return _instance;
}

void UI::AddElement(UIElement element)
{
	uiElements.push_back(element);
}

void UI::Reset()
{
	uiElements.clear();
}

void UI::DrawUI()
{
	for (int i = 0; i < uiElements.size(); i++)
	{
		uiElements[i].Draw();
	}
}

bool UI::Changed()
{
	if (uiChanged)
	{
		uiChanged = false;
		return true;
	}
	return false;
}

#pragma endregion

#pragma region UIElement

UI::UIElement::UIElement(int x, int y)
{
	position.X = x;
	position.Y = y;
}


UI::Button::Button(int x, int y, std::string label, void* interactFunction) : UIElement(x, y)
{
	buttonLabel = label;
	onInteract = interactFunction;
}

void UI::Button::Draw()
{
	printf("\033[%d;%lluH", position.Y, centreAligned ? position.X - (buttonLabel.length() / 2) : position.X);
	printf("\033[%d;%dm%s\033[0m", COLOUR_BACKGROUND(COLOUR_WHITE), COLOUR_BLACK, buttonLabel.c_str());
}

#pragma endregion

#pragma region UILayout

UI::UILayoutGroup::UILayoutGroup(int x, int y, int verticalSpacing, int elementNum, ...) : UIElement(x, y)
{
	elements = std::vector<UIElement>();

	va_list args;
	va_start(args, elementNum);

	for (int i = 0; i < elementNum; i++)
	{
		UIElement element = va_arg(args, UIElement);
		
		element.position.X = position.X;
		element.position.Y = position.Y + i + (verticalSpacing * i);

		elements.push_back(element);
	}

	InputHandler::GetInstance()->AddKey(VK_UP, OnSelectionChange);
}


void UI::UILayoutGroup::Draw()
{
	for (int i = 0; i < elements.size(); i++)
	{
		elements[i].Draw();
	}
}

#pragma endregion