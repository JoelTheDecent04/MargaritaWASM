#pragma once
#include "Game.h"
#include "GUI.h"
#include <vector>

class TitleScreen : public Level
{
	float fPeriod;
	std::shared_ptr<WidgetBitmap> logo;
	
public:
	TitleScreen();
	~TitleScreen();
	void LeftClick() override;
};