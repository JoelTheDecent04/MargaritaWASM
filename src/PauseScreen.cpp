#include "PauseScreen.h"
#include "Game.h"
#include "Space.h"
#include "TitleScreen.h"
#include "ControlsScreen.h"
#include "Utilities.h"
#include "Colours.h"

using namespace Game;

PauseScreen::PauseScreen()
{
	lPrevLevel = lCurrentLevel;

	gui->Add(std::make_shared<WidgetLabel>("Game Paused", Alignment::Centre, 44), 640, 70);

	gui->Add(std::make_shared<WidgetButton>(280, 100, [&] { LoadLevel(this->lPrevLevel); }, "Resume"), 500, 150);
	gui->Add(std::make_shared<WidgetButton>(280, 100, [] { LoadLevel(std::make_shared<ControlsScreen>()); }, "Controls"), 500, 275);
	gui->Add(std::make_shared<WidgetButton>(280, 100, [] { LoadLevel(std::static_pointer_cast<Level>(std::make_shared<TitleScreen>())); }, "Title Screen"), 500, 400);
	gui->Add(std::make_shared<WidgetButton>(280, 100, [] { Quit(); }, "Quit"), 500, 525);
}

void PauseScreen::render()
{
	if (lPrevLevel)
		lPrevLevel->Render();
	Graphics::FillRectangle(0.0f, 0.0f, fScaleH * 1280.0f, fScaleV * 720.0f, clrBlack, 0.7f);
}

void PauseScreen::LeftClick()
{
	gui->ClickOn(Mouse::X, Mouse::Y);
}

void PauseScreen::KeyDown(int key)
{
	if (key == SDL_SCANCODE_ESCAPE)
		LoadLevel(lPrevLevel);
}
