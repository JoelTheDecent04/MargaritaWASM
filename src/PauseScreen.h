#pragma once
#include "Game.h"
#include "GUI.h"
#include <vector>

class PauseScreen : public Level
{
public:
	std::shared_ptr<Level> lPrevLevel;

	PauseScreen();
	void render() override;
	void LeftClick() override;
	void KeyDown(int key) override;
};