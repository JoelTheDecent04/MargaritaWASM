#include "BombAnimation.h"

BombAnimation::BombAnimation(Bomb* bomb)
	: animation(Textures::BombAnimation, nFrames, 60, &nFrame)
{
	texture = Textures::BombAnimation;
	texture->fTextureDrawnWidth = 1.5f * 1.778f * (75.0f + 25.0f * bomb->nLevel);
	texture->fTextureDrawnHeight = 1.5f * (75.0f + 25.0f * bomb->nLevel);
	fX = bomb->fX - texture->fTextureDrawnWidth / 2;
	fY = bomb->fY - texture->fTextureDrawnHeight / 2;
	nFrame = 0;
}

void BombAnimation::Draw()
{
	Textures::BombAnimation->Draw(nFrame, fX - SpaceGame::RenderPosition(), fY);
}

bool BombAnimation::Update(float deltatime)
{
	animation.Update(deltatime);
	if (nFrame + 1 == nFrames) 
		return false;

	return true;
}
