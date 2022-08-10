#pragma once
#include "BackgroundObject.h"
#include "Animation.h"

class Comet : public BackgroundObject
{
	Animation animation;
	int nFrame;
	float fX, fY;
	float fSpeedX, fSpeedY;
public:
	Comet();
	void Draw() override;
	bool Update(float deltatime) override;
};