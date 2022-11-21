#include "TitleScreen.h"
#include "Game.h"
#include "Space.h"
#include "Utilities.h"
#include "Colours.h"

//static Texture* tButtons;
//static Texture* tTitlescreenBackground;

using namespace Game;

class WidgetTitle : public WidgetBitmap
{
	float time;
	float drawn_width;
	float drawn_x;
public:
	WidgetTitle(Texture* texture, int frame)
		:WidgetBitmap(0, 0, texture, frame)
	{
		drawn_width = 480;
		drawn_x = 0;
		time = 0.0f;
	}

	void Update(float deltatime, int, int) override
	{
		time += deltatime;
		time = fmod(time, 2.0f);

		float size = sin(time * M_PI);

		drawn_width = 480 + size * 70.0f;
		drawn_x = -size * 35.0f;
	}

	void Draw(int x, int y) override
	{
		tTexture->DrawDifferentSize(
			nFrame, x + drawn_x, y,
			drawn_width, 150);
	}
};

TitleScreen::TitleScreen()
{
	auto play_button_function = [] {
		if (SpaceGame::InstanceExists())
			LoadLevel(SpaceGame::InstanceAsSharedPtr());
		else
		{
			auto pSpaceGame = std::make_shared<SpaceGame>();
			SpaceGame::SetInstance(pSpaceGame);
			LoadLevel(pSpaceGame);
		}
	};

	auto controls = std::make_shared<WidgetGroup>(1280, 720);

	gui->Add(std::make_shared<WidgetBitmap>(1280, 720, Textures::TitleScreenBackground, 0), 0, 0);
	
	logo = std::make_shared<WidgetTitle>( Textures::TitleScreenLogo, 0);
	gui->Add(logo, 400, 30);

	controls->Add(std::make_shared<WidgetBitmapButton>(280, 125, play_button_function, Textures::TitleScreenButtons, 0), 500, 200);
	controls->Add(std::make_shared<WidgetBitmapButton>(280, 125, [] { Quit(); }, Textures::TitleScreenButtons, 1), 500, 500);

	gui->Add(controls, 0, 0);
}

TitleScreen::~TitleScreen()
{
}

void TitleScreen::LeftClick()
{
	gui->ClickOn(Mouse::X, Mouse::Y);
}
