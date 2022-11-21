#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <unordered_map>
#include "Graphics.h"

class Texture
{
	SDL_Texture* pTexture;
public:
	int nTextureWidth = 0, nTextureHeight = 0;
	float fTextureDrawnWidth = 0.0f, fTextureDrawnHeight = 0.0f;
	int nTexturesAcross = 0;

	Texture(const char* filename);
	Texture(const char* filename, int nTextureWidth, int nTextureHeight, float fTextureDrawnWidth, float fTextureDrawnHeight);
	Texture() { pTexture = nullptr; };
	~Texture();

	void Draw() const ;
	void Draw(int index, float x, float y, bool bRealCoordinates = false, float fAngle = 0.0f, uint8_t alpha = 255) const ;
	void DrawDifferentSize(int index, float x, float y, float width, float height, bool bRealCoordinates = false, float fAngle = 0.0f) const ;
	void DrawPanorama(float x) const;
	SDL_Texture* Handle() { return pTexture; };
};

class Textures
{
public:
	static Texture* Character;
	static Texture* Orb;
	static Texture* Background;
	static Texture* Laser;
	static Texture* LaserBeam;
	static Texture* Enemy;
	static Texture* Snail;
	static Texture* Crab;
	static Texture* Bomb;
	static Texture* Comet;
	static Texture* BombAnimation;
	static Texture* EnergyPowerup;
	static Texture* RegenerationPowerup;
	static Texture* Foreground;
	static Texture* Coin;
	static Texture* EnemyLaser;
	static Texture* Block;
	static Texture* TitleScreenButtons;
	static Texture* TitleScreenLogo;
	static Texture* TitleScreenBackground;

	static void LoadAll();
};