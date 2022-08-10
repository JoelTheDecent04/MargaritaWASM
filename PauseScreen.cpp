#include "PauseScreen.h"
#include "Game.h"
#include "Space.h"
#include "TitleScreen.h"
#include "ControlsScreen.h"
#include "Utilities.h"
#include "Colours.h"

using namespace Game;

PauseScreen::PauseScreen()
	:gui (1280, 720)
{
	lPrevLevel = lCurrentLevel;

	gui.Add(std::make_shared<WidgetLabel>("Game Paused", Alignment::Centre, 44), 640, 70);

	gui.Add(std::make_shared<WidgetButton>(280, 100, [](void* p) { LoadLevel(((PauseScreen*)p)->lPrevLevel); }, "Resume"), 500, 150);
	gui.Add(std::make_shared<WidgetButton>(280, 100, [](void* p) { LoadLevel(std::make_shared<ControlsScreen>()); }, "Controls"), 500, 275);
	gui.Add(std::make_shared<WidgetButton>(280, 100, [](void* p) { LoadLevel(std::static_pointer_cast<Level>(std::make_shared<TitleScreen>())); }, "Title Screen"), 500, 400);
	gui.Add(std::make_shared<WidgetButton>(280, 100, [](void* p) { Quit(); }, "Quit"), 500, 525);
}

void PauseScreen::Render()
{

	if (lPrevLevel)
		lPrevLevel->Render();
	Graphics::FillRectangle(0.0f, 0.0f, fScaleH * 1280.0f, fScaleV * 720.0f, clrBlack, 0.7f);

	//TODO fix
	//TextSize textsize;
	//Graphics::TextMetrics("Game Paused", Graphics::GetFont(44), textsize);
	//Graphics::WriteText("Game Paused", fScaleH * 640 - textsize.width / 2, fScaleV * 70, Graphics::GetFont(44));

	gui.Draw(0, 0);
}

void PauseScreen::Update(float deltatime)
{
	int nCursorX, nCursorY;
	GetRelativeMousePos(&nCursorX, &nCursorY);
	gui.Update(deltatime, nCursorX, nCursorY);
}

void PauseScreen::LeftClick()
{
	int nCursorX, nCursorY;
	GetRelativeMousePos(&nCursorX, &nCursorY);
	gui.ClickOn(nCursorX, nCursorY, this);
}

void PauseScreen::KeyDown(int key)
{
	if (key == SDL_SCANCODE_ESCAPE)
	{
		LoadLevel(lPrevLevel);
	}
}
