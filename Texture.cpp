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
	assert(ret == 0);

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

