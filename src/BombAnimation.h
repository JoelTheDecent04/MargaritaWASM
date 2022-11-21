#pragma once
#include "BackgroundObject.h"
#include "Texture.h"
#include "Bomb.h"
#include "Animation.h"

class BombAnimation : public BackgroundObject
{
	float fX, fY;
	int nFrame;
	Animation animation;

	static constexpr int nFrames = 21;
public:
	Texture* texture;
	BombAnimation(Bomb* bomb);
	void Draw() override;
	bool Update(float deltatime) override;
};