#pragma once
#include "Game.h"
#include "GUI.h"
#include <vector>

class PauseScreen : public Level
{
public:
	std::shared_ptr<Level> lPrevLevel;
	//std::vector<Button> vButtons;
	
	WidgetGroup gui;

	PauseScreen();
	void Render() override;
	void Update(float deltatime) override;
	void LeftClick() override;
	void KeyDown(int key) override;
};