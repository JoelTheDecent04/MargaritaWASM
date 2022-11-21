#pragma once
#include "Game.h"
#include "Space.h"
#include "GUI.h"
#include <vector>

class DeathScreen : public Level
{
public:
	DeathScreen();
	~DeathScreen();
	void render() override;
	void LeftClick() override;
};