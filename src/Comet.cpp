#include "Comet.h"
#include "Space.h"
#include "Utilities.h"

Comet::Comet()
	: animation(Textures::Comet, 42, 60, &nFrame)
{
	nFrame = 0;
	fX = SpaceGame::RenderPosition() + randomf() * 2560;
	fY = -100.0f;
	fSpeedX = -1200.0f;
	fSpeedY = 600.0f;
}

void Comet::Draw()
{
	Textures::Comet->Draw(nFrame, fX, fY);
}

bool Comet::Update(float deltatime)
{
	fX += fSpeedX * deltatime;
	fY += fSpeedY * deltatime;
	animation.Update(deltatime);
	return true;
}
