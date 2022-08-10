#pragma once
#include "BackgroundObject.h"
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

class TextObject : public BackgroundObject
{
public:
	float fOpacity;
	float fOpacityChangePerSecond;
	float fX, fY;
	float fSpeedX, fSpeedY;
	float nSize;
	SDL_Color* clrColour;
	std::string strText;
	TextObject(float fX, float fY, const std::string& strText);
	TextObject();
	bool Update(float deltatime) override;
	void Draw() override;
};