#include "DeathScreen.h"
#include "Game.h"
#include "Space.h"
#include "Colours.h"
#include <fstream>

using namespace Game;

void DeathScreen::render()
{
	SpaceGame::Instance().Render();
	Graphics::FillRectangle(0.0f, 0.0f, (float)nScreenWidth, (float)nScreenHeight, clrBlack, 0.7f);
}

void DeathScreen::LeftClick()
{
	gui->ClickOn(Mouse::X, Mouse::Y);
}

DeathScreen::DeathScreen()
{
	gui->Add(std::make_shared<WidgetLabel>("You Suck!", Alignment::Centre, 70), 640, 100);
	gui->Add(std::make_shared<WidgetButton>(280, 100, [] {
		auto pSpaceGame = std::make_shared<SpaceGame>();
		SpaceGame::SetInstance(pSpaceGame);
		LoadLevel(pSpaceGame);
	}, "New Game"), 500, 200);
	gui->Add(std::make_shared<WidgetButton>(280, 100, [] { Quit(); }, "Quit"), 500, 320);
}

DeathScreen::~DeathScreen()
{
	
}