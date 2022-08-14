#include "Particle.h"
#include "Graphics.h"
#include "Space.h"

Particle::Particle(float fSize, SDL_Color colour, float fX, float fY, float fSpeedX, float fSpeedY)
	: Entity(0, fX, fY)
{
	this->fSize = fSize;
	this->colour = colour;

	this->fSpeedX = fSpeedX;
	this->fSpeedY = fSpeedY;

	fWidth = 0.0f;
	fHeight = 0.0f;

	fCustomGravity = 700.0f;

	bCanCollide = false;
	bBouncy = false;
}

void Particle::Draw()
{
	RectRenderer().SetFilled().SetColour(colour).SetPosition(fX - SpaceGame::RenderPosition() - fSize / 2.0f, fY - fSize / 2.0f).SetSize(fSize, fSize).Draw();
}

void Particle::LandOnGround()
{
	fHealth = 0.0f; //Flag for removal
}

void Particle::CreateParticles(float fX, float fY, SDL_Color colour)
{
	for (int i = 0; i < 10; i++) {
		float fAngle = randomf() * 2 * M_PI;
		float fCos = cos(fAngle);
		float fSin = sin(fAngle);
		SpaceGame::Instance().AddEntity(new Particle(5.0f + randomf() * 10.0f, colour, fX + 10.0f * fCos, fY + 10.0f * fSin, fCos * 80.0f, fSin * 120.0f));
	}
}

