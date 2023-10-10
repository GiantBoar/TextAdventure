#pragma once

#include <iostream>
#include <vector>
#include <Windows.h>
#include <string>

class UI
{
private:
	static UI* _instance;

public:
	// base class for UI elements
	class UIElement
	{
	public:
		bool centreAligned = false;

		bool selected = false;
		COORD position;

		UIElement(int x, int y);

		virtual void Draw() { std::cout << "something is wrong"; }

		virtual void Interact(WORD key) { }
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

		int verticalSpacing = 0;
		int selectedIndex = 0;

		UILayoutGroup(int x, int y, int verticalSpacing);

		void AddElements(int elementNum, ...);

		void Interact(WORD key) override;

		void Draw() override;
	};

	static UI* GetInstance();

	bool uiChanged = false;

	UIElement *interactedElement = nullptr;

	std::vector<UIElement> uiElements;

	void AddElement(UIElement element);
	void Reset();

	void DrawUI();

	bool Changed();
};