#pragma once

#include <vector>
#include <Windows.h>
#include <string>

namespace UI
{
	// base class for UI elements
	class UIElement
	{
	public:
		bool centreAligned = false;

		bool selected = false;
		COORD position;

		UIElement(int x, int y);

		virtual void Draw() { }
	};


	class Button : public UIElement
	{
	public:
		std::string buttonLabel;

		void* onInteract = nullptr;

		Button(int x, int y, std::string label, void* interactFunction);

		void Draw() override;
	};


	class UILayoutGroup : public UIElement
	{
	public:
		std::vector<UIElement> elements;

		UILayoutGroup(int x, int y, int verticalSpacing, int elementNum, ...);

		void Draw() override;
	};
}