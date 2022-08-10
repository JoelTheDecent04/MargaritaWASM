#pragma once
#include "Entity.h"

class Particle : public Entity
{
protected:
	float fSize;
	SDL_Color colour;

public:
	Particle(float fSize, SDL_Color colour, float fX, float fY, float fSpeedX, float fSpeedY);
	virtual void Draw() override;
	void LandOnGround() override;

	static void CreateParticles(float fX, float fY, SDL_Color colour);
	
};