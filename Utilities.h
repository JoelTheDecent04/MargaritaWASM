#pragma once
#include <assert.h>
struct Rect
{
	int left, top, right, bottom;

	Rect() = default;
	Rect(int left, int top, int right, int bottom)
	{
		this->left = left;
		this->top = top;
		this->right = right;
		this->bottom = bottom;
	}
};

float distance(float ax, float ay, float bx, float by);
float randomf();
float random_off_screen();
void GetRelativeMousePos(int* x, int* y);
void DebugOut(const char* fmt, ...);
bool PointInRect(const Rect& rect, int x, int y);
bool KeyState(int key);

#define ErrorAndQuit(n, ...) { SDL_Log(n, ##__VA_ARGS__); SDL_Quit(); }

#define SAFE_DELETE(n) if (n) { delete n; n = nullptr; }

#define assert_not_reached() assert(0)