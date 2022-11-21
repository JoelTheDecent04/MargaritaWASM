#include "EntityHealthChangeText.h"
#include "Colours.h"
#include "Space.h"
#include <stdio.h>

EntityHealthChangeText::EntityHealthChangeText(Entity* entity, int nChange)
	: TextObject()
{
	bool healing = nChange > 0;
	char strTextBuffer[64];
	snprintf(strTextBuffer, sizeof(strTextBuffer), healing ? "+%d" : "%d", nChange);
	TextSize textsize;
	Graphics::TextMetrics(strTextBuffer, Graphics::GetFont(12), textsize);

	fOpacity = 1.0f;
	fOpacityChangePerSecond = -4.0f;
	fX = entity->fX - (textsize.width / 2);
	fY = entity->fY - (entity->texture->fTextureDrawnHeight / 2) - 4 - 12;
	fSpeedX = 0.0f;
	fSpeedY = -100.0f;
	
	nSize = 14;
	this->clrColour = healing ? &clrGreen : &clrRed;
	strText = std::string(strTextBuffer);
}