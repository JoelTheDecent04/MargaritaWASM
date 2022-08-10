#include "Utilities.h"
#include "Graphics.h"
#include "Game.h"
#include "Space.h"
#include <random>
#include <ctime>
#include <stdarg.h>

float distance(float ax, float ay, float bx, float by)
{
	float x = bx - ax;
	float y = by - ay;
	return sqrt(x * x + y * y);
}

float randomf()
{
	static std::mt19937 r = std::mt19937(time(0));
	return (float)r() / r.max();
}

float random_off_screen()
{
	float frandomfLocation = randomf() * (5120 - 1280);
	if (frandomfLocation > SpaceGame::RenderPosition()) frandomfLocation += 1280;
	return frandomfLocation;
}

void GetRelativeMousePos(int* x, int* y)
{
	int nCursorX, nCursorY;
	SDL_GetMouseState(&nCursorX, &nCursorY);

	*x = (nCursorX - nWindowWidth / 2) / fWindowScale + 1280 / 2;
	*y = (nCursorY - nWindowHeight / 2) / fWindowScale + 720 / 2;
}

bool PointInRect(const Rect& rect, int x, int y) //TODO make SDL_PointInRect
{
	return (x > rect.left && y > rect.top && x < rect.right && y < rect.bottom);
}

void DebugOut(const char* fmt, ...) {
	/*char s[1025];
	va_list args;
	va_start(args, fmt);
	wvsnprintf(s, fmt, args);
	va_end(args);
	OutputDebugString(s);*/
}

bool KeyState(int key)
{
	return (/*key < pKeyStatesLength&&*/ Game::pKeyStates[key]);
}