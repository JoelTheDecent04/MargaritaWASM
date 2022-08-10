#pragma once
#include "Game.h"
#include "GUI.h"
#include <vector>

class TitleScreen : public Level
{
	WidgetGroup gui;
	float fPeriod;
	std::shared_ptr<WidgetBitmap> logo;
	
public:
	TitleScreen();
	~TitleScreen();
	void Render() override;
	void Update(float deltatime) override;
	void LeftClick() override;
};