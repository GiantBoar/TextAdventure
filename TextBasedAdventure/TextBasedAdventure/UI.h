#pragma once

#include <vector>
#include <Windows.h>
#include <string>

class UI
{
public:
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

		int selectedIndex = 0;

		UILayoutGroup(int x, int y, int verticalSpacing, int elementNum, ...);

		void OnSelectionChange(int change);

		void Draw() override;
	};


	static UI* _instance;
	static UI* GetInstance();

	bool uiChanged = false;

	std::vector<UIElement> uiElements;

	void AddElement(UIElement element);
	void Reset();

	void DrawUI();

	bool Changed();
};