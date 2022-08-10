#pragma once
#include "Game.h"
#include "GUI.h"
#include <vector>
#include <string>

class PauseScreen;

class ControlsScreen : public Level
{
	std::shared_ptr<Level> lPreviousLevel;
	struct Control
	{
		int* nKey1, * nKey2;
		const char* strDescription;
		Control(int* nKey1, int* nKey2, const char* strDescription) { this->nKey1 = nKey1; this->nKey2 = nKey2; this->strDescription = strDescription; };
	};
	std::vector<Control> vControls;
	std::vector<Button> vButtons;
	int nButtonHover;

	int* keyToChange;

	WidgetGroup gui;

public:
	ControlsScreen();
	~ControlsScreen();
	void Render() override;
	void Update(float deltatime) override;
	void KeyDown(int key) override;
	void LeftClick() override;
	
};