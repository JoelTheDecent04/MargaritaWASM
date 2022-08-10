#pragma once
#include "Colours.h"
#include "Utilities.h"
#include <cassert>
#include <mutex>
#include <SDL.h>
#include <SDL_ttf.h>
#include <sstream>

struct TextSize
{
	int width, height;
};

struct RectSize
{
	int width, height;
	RectSize(int width, int height) {
		this->width = width;
		this->height = height;
	}
	RectSize& operator+(const RectSize& rs) {
		width += rs.width;
		height += rs.height;
		return *this;
	}
};


namespace Graphics {	
	bool Initialise(SDL_Window* window);
	void Close();
	void BeginDraw();
	void EndDraw();
	void Clear(SDL_Color colour);
	void WriteText(const char* text, float x, float y, TTF_Font* pFont, SDL_Color clrColour = clrWhite, float fOpacity = 1.0f);
	void TextMetrics(const char* text, TTF_Font* pFont, TextSize& ts);
	void DrawRectangle(float fX, float fY, float fW, float fH, SDL_Color cColour, float fOpacity = 1.0f, float fThickness = 1.0f);
	void FillRectangle(float fX, float fY, float fW, float fH, SDL_Color cColour, float fOpacity = 1.0f);

	void Resize();
	void ResizeGUI();

	enum {
		Absolute = false,
		Relative = true
	};
	TTF_Font* GetFont(int size, bool relative = true);

	extern SDL_Renderer* pRenderer;
	extern SDL_Window* pWindow;
}

class EndStream {};

enum class Alignment {
	TopLeft,
	Centre,
	TopRight,
	BottomRight,
	TopCentre,
	BottomCentre,
	BottomLeft,
	LeftCentre,
	RightCentre
};

class TextRenderer
{
	float x = 0.0f, y = 0.0f;
	int size = 16;
	SDL_Color colour = clrWhite;
	Alignment align = Alignment::TopLeft;
	bool absolute = false;
public:
	TextRenderer() {};
	TextRenderer& SetPosition(float x, float y) {
		this->x = x;
		this->y = y;
		return *this;
	}
	TextRenderer& SetSize(int size) {
		this->size = size;
		return *this;
	}
	TextRenderer& SetAlignment(Alignment align) {
		this->align = align;
		return *this;
	}
	TextRenderer& SetAbsoluteCoordinates() {
		absolute = true;
		return *this;
	}
	TextRenderer& SetColour(SDL_Color colour) {
		this->colour = colour;
		return *this;
	}
	void SetText(const std::stringstream& ss);
};

class GraphicsStreamText
{
	static const int BUFFER_LENGTH = 64;
	char buf[BUFFER_LENGTH];
	size_t offset { 0 };
	float x, y;
	int text_size;
	SDL_Color colour;
	Alignment align;

	SDL_Texture* pTextTexture { nullptr };

	void generate_texture();
	
public:
	GraphicsStreamText(float x, float y, int text_size, SDL_Color colour = clrWhite, Alignment align = Alignment::TopLeft) {
		this->x = x;
		this->y = y;
		this->align = align;
		this->text_size = text_size;
		this->colour = colour;
		buf[0] = 0;
	}
	GraphicsStreamText& operator<<(const char* source) {
		strncpy(buf + offset, source, BUFFER_LENGTH - offset);
		//strcpy_s(buf + offset, BUFFER_LENGTH - offset, source);
		offset += strlen(source);
		return *this;
	}
	GraphicsStreamText& operator<<(int i) {
		offset += snprintf(buf + offset, BUFFER_LENGTH - offset, "%d", i);
		//offset += sprintf_s(buf + offset, BUFFER_LENGTH - offset, "%d", i);
		return *this;
	}
	void operator<<(EndStream) {
		Draw();
	}
	RectSize Size();

	void Draw();
};

class GraphicsStreamRect
{
public:
	GraphicsStreamRect(float x, float y, RectSize size, SDL_Color colour, Alignment align);
};

class RectRenderer
{
	float x = 0.0f, y = 0.0f;
	float width = 10.0f, height = 10.0f;
	SDL_Color colour = clrWhite;
	Alignment align = Alignment::TopLeft;
	bool absolute = false;
	bool filled = false;
public:
	RectRenderer() {}

	RectRenderer& SetPosition(float x, float y) {
		this->x = x;
		this->y = y;
		return *this;
	}
	RectRenderer& SetSize(float width, float height) {
		this->width = width;
		this->height = height;
		return *this;
	}
	RectRenderer& SetColour(SDL_Color colour) {
		this->colour = colour;
		return *this;
	}
	RectRenderer& SetAbsoluteCoordinates() {
		absolute = true;
		return *this;
	}
	RectRenderer& SetFilled() {
		filled = true;
		return *this;
	}
	RectRenderer& SetAlignment(Alignment align) {
		this->align = align;
		return *this;
	}
	void Draw();



};

#define TO_SCREEN(x, y) x * (float)nScreenWidth / 1280.0f, y * (float)nScreenHeight / 720.0f

using TextBuf = std::stringstream;