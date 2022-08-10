#include "DeathScreen.h"
#include "Game.h"
#include "Space.h"
#include "Colours.h"
#include <fstream>

using namespace Game;

void DeathScreen::Render()
{
	SpaceGame::Instance().Render();

	Graphics::FillRectangle(0.0f, 0.0f, (float)nScreenWidth, (float)nScreenHeight, clrBlack, 0.7f);

	gui.Draw(0, 0);
}

void DeathScreen::Update(float deltatime)
{
	int nCursorX, nCursorY;
	GetRelativeMousePos(&nCursorX, &nCursorY);
	gui.Update(deltatime, nCursorX, nCursorY);
}

void DeathScreen::LeftClick()
{
	int nCursorX, nCursorY;
	GetRelativeMousePos(&nCursorX, &nCursorY);
	gui.ClickOn(nCursorX, nCursorY, this);
}

DeathScreen::DeathScreen()
	: gui(1280, 720)
{
	gui.Add(std::make_shared<WidgetLabel>("You Suck!", Alignment::Centre, 70), 640, 100);
	gui.Add(std::make_shared<WidgetButton>(280, 100, [](void*) {
		auto pSpaceGame = std::make_shared<SpaceGame>();
		SpaceGame::SetInstance(pSpaceGame);
		LoadLevel(pSpaceGame);
	}, "New Game"), 500, 200);
	gui.Add(std::make_shared<WidgetButton>(280, 100, [](void*) { Quit(); }, "Quit"), 500, 320);
}

DeathScreen::~DeathScreen()
{
	
}