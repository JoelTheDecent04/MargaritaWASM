#include "Animation.h"

Animation::Animation(Texture* texture, int nNumFrames, float fFrameRate, int* pnFrame)
{
	this->texture = texture;
	this->nNumFrames = nNumFrames;
	this->pnFrame = pnFrame;
	this->fFrameRate = fFrameRate;
	fSecondsUntilNextFrame = 0.0f;
	bAnimationRunning = true;
}

void Animation::Update(float deltatime)
{
	if (bAnimationRunning)
	{
		fSecondsUntilNextFrame -= deltatime;
		if (fSecondsUntilNextFrame <= 0.0)
		{
			fSecondsUntilNextFrame += 1.0f / fFrameRate;
			(*pnFrame)++;
			if (*pnFrame >= nNumFrames) *pnFrame = 0;
		}
	}
}
