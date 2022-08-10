#include <cassert>
#include <mutex>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_rect.h>
#include "Game.h"
#include "Graphics.h"
#include "Colours.h"
#include "Utilities.h"

#include <unordered_map>

#define TESTING

namespace Graphics {
	
	bool init;
	
	SDL_Window* pWindow;
	SDL_Renderer* pRenderer;
	
	//static const char* strFontFile = "resources/Margarita-Regular.ttf";
	static const char* strFontFile = "resources/TitilliumWeb-Regular.ttf";

	bool Initialise(SDL_Window* w)
	{
		pWindow = w;
		//SDL_CreateWindowAndRenderer(1280, 720, 0, &pWindow, &pRenderer);
		pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); //SDL_RENDERER_PRESENTVSYNC
		
		SDL_RenderSetLogicalSize(pRenderer, 1280, 720);
		if (pRenderer == nullptr)
		{
			ErrorAndQuit("Failed to create renderer: %s", SDL_GetError());
			return false;
		}

		SDL_Surface* pWindowIconSurface = IMG_Load("resources/icon.png");
		if (pWindowIconSurface)
		{
			SDL_SetWindowIcon(pWindow, IMG_Load("resources/icon.png"));
			SDL_FreeSurface(pWindowIconSurface);
		}

		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2"))
			SDL_Log("Failed to set best render scale quality: %s", SDL_GetError());

		if (SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_BLEND) != 0)
			SDL_Log("Failed to set render draw blend mode: %s", SDL_GetError());



		init = true;
		return true;
	}

	void Close()
	{
	}

	void BeginDraw()
	{
	}

	void EndDraw()
	{
		SDL_RenderPresent(pRenderer);
	}

	void Clear(SDL_Color clrColour)
	{
		SDL_SetRenderDrawColor(pRenderer, clrColour.r, clrColour.b, clrColour.g, clrColour.a);
		SDL_RenderClear(pRenderer);
	}

	void WriteText(const char* text, float x, float y, TTF_Font* pFont, SDL_Color clrColour, float fOpacity)
	{
		if (text[0] == '\n')
			text++;

		SDL_Surface* pTextSurface = TTF_RenderText_Blended_Wrapped(pFont, text, clrColour, nScreenWidth);
		if (pTextSurface == nullptr) return;
		SDL_Texture* pTextTexture = SDL_CreateTextureFromSurface(pRenderer, pTextSurface);
		if (pTextTexture == nullptr) return;
		int nWidth, nHeight;
		SDL_QueryTexture(pTextTexture, nullptr, nullptr, &nWidth, &nHeight);
		SDL_FRect rDest = { x, y, (float)nWidth, (float)nHeight}; //TODO fix
		SDL_RenderCopyF(pRenderer, pTextTexture, nullptr, &rDest);

		SDL_FreeSurface(pTextSurface);
		SDL_DestroyTexture(pTextTexture);
	}

	void TextMetrics(const char* text, TTF_Font* pFont, TextSize& ts)
	{
		bool bDoubleHeight = false;
		if (text[0] == '\n')
		{
			text++;
			bDoubleHeight = true;
		}

		TTF_SizeText(pFont, text, &ts.width, &ts.height);
		if (bDoubleHeight)
			ts.height *= 2;
	}
	
	void DrawRectangle(float fX, float fY, float fW, float fH, SDL_Color cColour, float fOpacity, float thickness)
	{
		uint8_t nOpacity = fOpacity * 255.0f;
		SDL_SetRenderDrawColor(pRenderer, cColour.r, cColour.g, cColour.b, nOpacity);
		SDL_FRect rRect = { fX, fY, fW, fH }; //thickness
		SDL_RenderDrawRectF(pRenderer, &rRect);

		#ifdef TESTING
		SDL_RenderFlush(pRenderer);
		#endif
	}

	void FillRectangle(float fX, float fY, float fW, float fH, SDL_Color cColour, float fOpacity)
	{
		uint8_t nOpacity = fOpacity * 255.0f;
		SDL_SetRenderDrawColor(pRenderer, cColour.r, cColour.g, cColour.b, nOpacity);
		SDL_FRect rRect = { fX, fY, fW, fH };
		SDL_RenderFillRectF(pRenderer, &rRect);

		#ifdef TESTING
		SDL_RenderFlush(pRenderer);
		#endif
	}

	static std::unordered_map<int, TTF_Font*> fonts_relative;
	static std::unordered_map<int, TTF_Font*> fonts_absolute;

	void Resize()
	{
		fonts_relative.clear();

	}

	void ResizeGUI()
	{
		fonts_absolute.clear();
	}

	TTF_Font* GetFont(int size, bool relative)
	{
		//TODO change to contains
		if (relative)
		{
			if (fonts_relative.find(size) == fonts_relative.end())
			{
				TTF_Font* pFont = TTF_OpenFont(strFontFile, (int)(fWindowScale * size));
				fonts_relative[size] = pFont;
				return pFont;
			}
			return fonts_relative[size];
		}
		else
		{
			if (fonts_absolute.find(size) == fonts_absolute.end())
			{
				TTF_Font* pFont = TTF_OpenFont(strFontFile, (int)(fGUIScale * size));
				fonts_absolute[size] = pFont;
				return pFont;
			}
			return fonts_absolute[size];
		}
	}

}

void GraphicsStreamText::generate_texture()
{
	if (pTextTexture == nullptr) {
		SDL_Surface* pTextSurface = TTF_RenderText_Blended(GetFont(text_size, Graphics::Absolute), buf, colour);
		if (pTextSurface == nullptr)
			return;
		pTextTexture = SDL_CreateTextureFromSurface(Graphics::pRenderer, pTextSurface);
		SDL_FreeSurface(pTextSurface);
	}
}

void GraphicsStreamText::Draw() {
	assert(offset < BUFFER_LENGTH);
	buf[offset] = 0;

	generate_texture();
	if (pTextTexture == nullptr)
		return;

	int nWidth, nHeight;
	SDL_QueryTexture(pTextTexture, nullptr, nullptr, &nWidth, &nHeight);
	SDL_FRect rDest;
	switch (align) {
		case Alignment::TopLeft:
			rDest = { x, y, (float)nWidth, (float)nHeight };
			break;
		case Alignment::Centre:
			rDest = { x - (float)nWidth / 2, y - (float)nWidth / 2, (float)nWidth, (float)nHeight };
			break;
		case Alignment::BottomRight:
			rDest = { x - (float)nWidth, y - (float)nHeight, (float)nWidth, (float)nHeight };
			break;
		case Alignment::TopRight:
			rDest = { x - (float)nWidth, y, (float)nWidth, (float)nHeight };
			break;
		case Alignment::TopCentre:
			rDest = { x - (float)nWidth / 2, y, (float)nWidth, (float)nHeight };
			break;
		case Alignment::BottomCentre:
			rDest = { x - (float)nWidth / 2, y - (float)nHeight, (float)nWidth, (float)nHeight };
			break;
		case Alignment::BottomLeft:
			rDest = { x , y - (float)nHeight, (float)nWidth, (float)nHeight };
			break;
		default:
			assert(0);
	}

	SDL_RenderCopyF(Graphics::pRenderer, pTextTexture, nullptr, &rDest);
	SDL_DestroyTexture(pTextTexture);
}

RectSize GraphicsStreamText::Size()
{
	generate_texture();
	int nWidth, nHeight;
	SDL_QueryTexture(pTextTexture, nullptr, nullptr, &nWidth, &nHeight);
	return { nWidth, nHeight };
}

GraphicsStreamRect::GraphicsStreamRect(float x, float y, RectSize size, SDL_Color colour, Alignment align)
{
	SDL_FRect rDest;
	switch (align) {
	case Alignment::TopLeft:
		rDest = { x, y, (float)size.width, (float)size.height };
		break;
	case Alignment::Centre:
		rDest = { x - (float)size.width / 2, y - (float)size.height / 2, (float)size.width, (float)size.height };
		break;
	case Alignment::BottomRight:
		rDest = { x - (float)size.width, y - (float)size.height, (float)size.width, (float)size.height };
		break;
	case Alignment::TopRight:
		rDest = { x - (float)size.width, y, (float)size.width, (float)size.height };
		break;
	case Alignment::TopCentre:
		rDest = { x - (float)size.width / 2, y, (float)size.width, (float)size.height };
		break;
	case Alignment::BottomLeft:
		rDest = { x , y - (float)size.height, (float)size.width, (float)size.height };
		break;
	case Alignment::LeftCentre:
		rDest = { x, y - (float)size.height / 2, (float)size.width, (float)size.height };
		break;
	case Alignment::RightCentre:
		rDest = { x - (float)size.width, y - (float)size.height / 2, (float)size.width, (float)size.height };
		break;
	default:
		assert(0);
	}
	SDL_SetRenderDrawColor(Graphics::pRenderer, colour.r, colour.g, colour.b, 255.0f);
	auto ret = SDL_RenderFillRectF(Graphics::pRenderer, &rDest);
	if (ret == -1)
		SDL_Log("Failed to fill rect\n");
}

void TextRenderer::SetText(const std::stringstream& ss)
{
	if (!absolute) {
		x = x * fScaleH;
		y = y * fScaleV;
	}

	auto* pTextSurface = TTF_RenderText_Blended(Graphics::GetFont(size, !absolute), ss.str().c_str(), colour);
	if (pTextSurface == nullptr)
		return;
	auto* pTextTexture = SDL_CreateTextureFromSurface(Graphics::pRenderer, pTextSurface);
	SDL_FreeSurface(pTextSurface);

	int nWidth, nHeight;
	SDL_QueryTexture(pTextTexture, nullptr, nullptr, &nWidth, &nHeight);

	if (absolute) {
		nWidth /= fGUIScale;
		nHeight /= fGUIScale;
	}
	else {
		nWidth /= fWindowScale;
		nHeight /= fWindowScale;
	}

	SDL_FRect rDest;
	switch (align) {
	case Alignment::TopLeft:
		rDest = { x, y, (float)nWidth, (float)nHeight };
		break;
	case Alignment::Centre:
		rDest = { x - (float)nWidth / 2, y - (float)nHeight / 2, (float)nWidth, (float)nHeight };
		break;
	case Alignment::BottomRight:
		rDest = { x - (float)nWidth, y - (float)nHeight, (float)nWidth, (float)nHeight };
		break;
	case Alignment::TopRight:
		rDest = { x - (float)nWidth, y, (float)nWidth, (float)nHeight };
		break;
	case Alignment::TopCentre:
		rDest = { x - (float)nWidth / 2, y, (float)nWidth, (float)nHeight };
		break;
	case Alignment::BottomCentre:
		rDest = { x - (float)nWidth / 2, y - (float)nHeight, (float)nWidth, (float)nHeight };
		break;
	case Alignment::BottomLeft:
		rDest = { x , y - (float)nHeight, (float)nWidth, (float)nHeight };
		break;
	case Alignment::LeftCentre:
		rDest = { x, y - (float)nHeight / 2, (float)nWidth, (float)nHeight };
		break;
	case Alignment::RightCentre:
		rDest = { x - (float)nWidth, y - (float)nHeight / 2, (float)nWidth, (float)nHeight };
		break;
	default:
		assert(0);
	}

	SDL_RenderCopyF(Graphics::pRenderer, pTextTexture, nullptr, &rDest);
	SDL_DestroyTexture(pTextTexture);
}

void RectRenderer::Draw()
{
	SDL_FRect rDest;
	switch (align) {
	case Alignment::TopLeft:
		rDest = { x, y, (float)width, (float)height };
		break;
	case Alignment::Centre:
		rDest = { x - (float)width / 2, y - (float)height / 2, (float)width, (float)height };
		break;
	case Alignment::BottomRight:
		rDest = { x - (float)width, y - (float)height, (float)width, (float)height };
		break;
	case Alignment::TopRight:
		rDest = { x - (float)width, y, (float)width, (float)height };
		break;
	case Alignment::TopCentre:
		rDest = { x - (float)width / 2, y, (float)width, (float)height };
		break;
	case Alignment::BottomLeft:
		rDest = { x , y - (float)height, (float)width, (float)height };
		break;
	case Alignment::LeftCentre:
		rDest = { x, y - (float)height / 2, (float)width, (float)height };
		break;
	case Alignment::RightCentre:
		rDest = { x - (float)width, y - (float)height / 2, (float)width, (float)height };
		break;
	default:
		assert(0);
	}
	if (!absolute) {
		rDest.x *= fScaleH;
		rDest.w *= fScaleH;
		rDest.y *= fScaleV;
		rDest.h *= fScaleV;
	}

	SDL_SetRenderDrawColor(Graphics::pRenderer, colour.r, colour.g, colour.b, 255.0f);
	if (filled)
		SDL_RenderFillRectF(Graphics::pRenderer, &rDest);
	else
		SDL_RenderDrawRectF(Graphics::pRenderer, &rDest);

	SDL_RenderFlush(Graphics::pRenderer);
}
