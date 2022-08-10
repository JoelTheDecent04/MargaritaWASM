#include "TextObject.h"
#include "Graphics.h"
#include "Game.h"
#include "Space.h"
#include "Colours.h"

TextObject::TextObject(float fX, float fY, const std::string& strText)
{
	fOpacity = 1.0f;
	fOpacityChangePerSecond = 0.0f;
	this->fX = fX;
	this->fY = fY;
	fSpeedX = 0.0f;
	fSpeedY = 0.0f;
	this->strText = strText;
	clrColour = &clrBlack;
	nSize = 14;
}

TextObject::TextObject() {}

bool TextObject::Update(float deltatime)
{
	fX += fSpeedX * deltatime;
	fY += fSpeedY * deltatime;
	fOpacity += fOpacityChangePerSecond * deltatime;
	if (fOpacity < 0.0f) return false;
	return true;
}

void TextObject::Draw()
{
	Graphics::WriteText(strText.c_str(), fScaleH * (fX - SpaceGame::RenderPosition()), fScaleV * (fY), Graphics::GetFont(nSize), *clrColour, fOpacity);
}
