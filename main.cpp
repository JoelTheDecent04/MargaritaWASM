#include <time.h>
#include <cassert>
#include <thread>
#include "Game.h"
#include "Space.h"
#include "TitleScreen.h"
#include "Utilities.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#ifdef _WIN32
#include <Windows.h>
#endif

using namespace Game;

extern "C" int main(int argc, char** argv)
{
	srand((unsigned int)time(0));

#ifdef _WIN32
	SDL_Log("Setting DPI awareness");
	if (!SetProcessDPIAware())
		SDL_Log("Failed to set DPI awareness");
#endif

	int ret = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	if (ret < 0) ErrorAndQuit("Couldn't initialise SDL\n");
	ret = IMG_Init(IMG_INIT_PNG);
	if (ret < 0) ErrorAndQuit("Couldn't initialise SDL Image\n");
	ret = TTF_Init();
	if (ret < 0) ErrorAndQuit("Couldn't initialise SDL TTF\n");

	SDL_Log("Test message\n");

	SDL_Window* pWindow = SDL_CreateWindow("Margarita (Beta)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, nScreenWidth, nScreenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

	
	if (pWindow == nullptr)
		ErrorAndQuit("Couldn't create window: %s", SDL_GetError());

	if (Graphics::Initialise(pWindow) == false)
		ErrorAndQuit("Failed to initialise graphics: %s", SDL_GetError());

	GameMain();

	return 0;
}

bool DoEvents()
{
	SDL_Event event = { 0 };

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			Quit();
			SDL_Quit();
			return false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
				LeftClick();
			break;
		case SDL_TEXTINPUT:
				KeyPress(event.text.text);
			break;
		case SDL_KEYDOWN:
				KeyDown(event.key.keysym.scancode);
				break;
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				SDL_Log("Resized");
				nWindowWidth = event.window.data1;
				nWindowHeight = event.window.data2;

				fWindowScale = fmin(nWindowWidth / 1280.0f, nWindowHeight / 720.0f);
				fGUIScale = fWindowScale;

				//fScaleH = (int)nScreenWidth / 1280.0f;
				//fScaleV = (int)nScreenHeight / 720.0f;
				Graphics::Resize();
				Graphics::ResizeGUI();
			}
			break;
		}
	}
	return true;
}
