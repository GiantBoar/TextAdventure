#include "UI.h"
#include "Graphics.h"

using namespace UI;

UIElement::UIElement(int x, int y)
{
	position.X = x;
	position.Y = y;
}



Button::Button(int x, int y, std::string label, void* interactFunction) : UIElement(x, y)
{
	buttonLabel = label;
	onInteract = interactFunction;
}

void Button::Draw()
{
	printf("\033[%d;%lluH", position.Y, centreAligned ? position.X - (buttonLabel.length() / 2) : position.X);
	printf("\033[%d;%dm%s\033[0m", COLOUR_BACKGROUND(COLOUR_WHITE), COLOUR_BLACK, buttonLabel.c_str());
}



UILayoutGroup::UILayoutGroup(int x, int y, int verticalSpacing, int elementNum, ...) : UIElement(x, y)
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
}

void UILayoutGroup::Draw()
{
	for (int i = 0; i < elements.size(); i++)
	{
		elements[i].Draw();
	}
}