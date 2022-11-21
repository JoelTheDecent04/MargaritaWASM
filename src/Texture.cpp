#include "Texture.h"
#include "Game.h"

Texture::Texture(const char* filename)
{
	std::string str = std::string("resources/") + filename;
	pTexture = IMG_LoadTexture(Graphics::pRenderer, str.c_str());
	const char* error = SDL_GetError();
	if (pTexture == nullptr)
	{
		SDL_Log("Couldn't open texture \"%s\" Error: %s", str.c_str(), SDL_GetError());
	}
}
Texture::Texture(const char* filename, int nTextureWidth, int nTextureHeight, float fTextureDrawnWidth, float fTextureDrawnHeight)
	: Texture(filename)
{
	if (pTexture == nullptr) return;
	this->nTextureHeight = nTextureHeight;
	this->nTextureWidth = nTextureWidth;
	this->fTextureDrawnWidth = fTextureDrawnWidth;
	this->fTextureDrawnHeight = fTextureDrawnHeight;

	int nTextureFileWidth;
	SDL_QueryTexture(pTexture, nullptr, nullptr, &nTextureFileWidth, nullptr);
	nTexturesAcross = nTextureFileWidth / nTextureWidth;
}
Texture::~Texture()
{
	SDL_DestroyTexture(pTexture);
}

void Texture::Draw() const
{
	if (pTexture)
		SDL_RenderCopy(Graphics::pRenderer, pTexture, nullptr, nullptr);
}
void Texture::DrawDifferentSize(int index, float x, float y, float width, float height, bool bRealCoordinates, float fAngle) const 
{
	if (pTexture == nullptr) return;
	
	SDL_Rect rSrc;
	if (nTexturesAcross != 0)
	{
		rSrc = {
			((index % nTexturesAcross) * nTextureWidth),
			((index / nTexturesAcross) * nTextureHeight),
			nTextureWidth,
			nTextureHeight };
	}
	else
		rSrc = { 0, 0, nTextureWidth, nTextureHeight };

	SDL_FRect rDest = { x, y, width, height };

	if (!bRealCoordinates)
	{
		rDest.y *= fScaleV;
		rDest.h *= fScaleV;
		rDest.x *= fScaleH;
		rDest.w *= fScaleH;
	}

	if (fAngle != 0.0f)
	{
		SDL_FPoint point = { fScaleH * (x + width / 2), fScaleV * (y + height / 2) };
			SDL_RenderCopyExF(Graphics::pRenderer, pTexture, &rSrc, &rDest,
				fAngle, &point, SDL_FLIP_NONE);
	}
	else
	{
		int r = SDL_RenderCopyF(Graphics::pRenderer, pTexture, &rSrc, &rDest);
	}

}
void Texture::Draw(int index, float x, float y, bool bRealCoordinates, float fAngle, uint8_t alpha) const
{

	if (pTexture == nullptr) return;

	SDL_Rect rSrc;
	if (nTexturesAcross != 0)
	{
		rSrc = {
			((index % nTexturesAcross) * nTextureWidth),
			((index / nTexturesAcross) * nTextureHeight),
			nTextureWidth,
			nTextureHeight };
	}
	else
		rSrc = { 0, 0, nTextureWidth, nTextureHeight };

	SDL_FRect rDest = { x, y, fTextureDrawnWidth, fTextureDrawnHeight };
	if (!bRealCoordinates)
	{
		rDest.y *= fScaleV;
		rDest.h *= fScaleV;
		rDest.x *= fScaleH;
		rDest.w *= fScaleH;
	}

	int ret = SDL_SetTextureAlphaMod(pTexture, alpha);
	//assert(ret == 0);

	if (fAngle != 0.0f)
	{
		SDL_FPoint point = { fTextureDrawnWidth / 2, fTextureDrawnHeight / 2 };//{ nTextureWidth / 2, nTextureHeight / 2 };
		SDL_RenderCopyExF(Graphics::pRenderer, pTexture, &rSrc, &rDest,
			fAngle, &point, SDL_FLIP_NONE);
	}
	else
		SDL_RenderCopyF(Graphics::pRenderer, pTexture, &rSrc, &rDest);
}
void Texture::DrawPanorama(float x) const
{
	if (pTexture == nullptr) return;

	float fBitmapScale = (float)nTextureHeight / 720.0f;
	SDL_Rect rSrc = { (int)(fBitmapScale * x), 0, (int)(fBitmapScale * 1280), nTextureHeight };
	SDL_FRect rDest{ 0, 0, (float)nScreenWidth, (float)nScreenHeight };

	SDL_RenderCopyF(Graphics::pRenderer, pTexture, &rSrc, &rDest);
}

Texture* Textures::Character;
Texture* Textures::Orb;
Texture* Textures::Background;
Texture* Textures::Laser;
Texture* Textures::LaserBeam;
Texture* Textures::Enemy;
Texture* Textures::Snail;
Texture* Textures::Crab;
Texture* Textures::Bomb;
Texture* Textures::Comet;
Texture* Textures::BombAnimation;
Texture* Textures::EnergyPowerup;
Texture* Textures::RegenerationPowerup;
Texture* Textures::Foreground;
Texture* Textures::Coin;
Texture* Textures::EnemyLaser;
Texture* Textures::Block;
Texture* Textures::TitleScreenButtons;
Texture* Textures::TitleScreenLogo;
Texture* Textures::TitleScreenBackground;

void Textures::LoadAll()
{
	Character =		new Texture("player.png", 186, 116, 83, 58);
	Orb =				new Texture("orb.png", 2497, 2497, 32, 32);
	Background =		new Texture("background.png", 8192, 1152, 5120, 720);
	Laser =			new Texture("Laser.png", 32, 32, 32.0f, 32.0f);
	LaserBeam =		new Texture("LaserBeam.png", 2569, 765, 32, 10);
	Enemy =			new Texture("enemy.png", 77, 110, 38, 55);
	Crab =				new Texture("crab.png", 700, 350, 96, 48);
	Bomb =				new Texture("bomb.png", 2218, 2223, 32.0f, 32.0f);
	Comet =			new Texture("comet.png", 640, 360, 100, 50);
	BombAnimation =	new Texture("bomb_animation.png", 1280, 720, 100.0f, 100.0f);
	EnergyPowerup =	new Texture("energy_powerup.png", 2600, 2600, 32.0f, 32.0f);
	RegenerationPowerup = new Texture("regen_powerup.png", 2415, 2415, 32.0f, 32.0f);
	Foreground =		new Texture("foreground.png", 8192, 204, 5120, 127.33f);
	Coin =				new Texture("coin.png", 96, 96, 32.f, 32.f);
	EnemyLaser = 		new Texture("enemy_laser.png", 100, 48, 48.f, 24.f);
	Block = 			new Texture("blocks.png", 32, 32, 32.f, 32.f);
	Snail = 			new Texture("snail.png", 275, 274, 60, 59);

	TitleScreenButtons = new Texture("buttons.png", 764, 343, 280, 125);
	TitleScreenLogo = new Texture("titlescreen.png", 1603, 625, 320.0f, 125.0f);
	TitleScreenBackground = new Texture("titlescreen_background.png", 3022, 1700, 1280.0f, 720.0f);
}