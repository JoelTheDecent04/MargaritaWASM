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
	: gui(1280, 720)
{
	static Texture* tButtons = new Texture("buttons.png", 764, 343, 280, 125);
	static Texture* tTitleScreenLogo = new Texture("titlescreen.png", 1603, 625, 320.0f, 125.0f);
	static Texture* tTitlescreenBackground = new Texture("titlescreen_background.png", 3022, 1700, 1280.0f, 720.0f);

	auto play_button_function = [](void*) {
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

	gui.Add(std::make_shared<WidgetBitmap>(1280, 720, tTitlescreenBackground, 0), 0, 0);
	
	logo = std::make_shared<WidgetTitle>( tTitleScreenLogo, 0);
	gui.Add(logo, 400, 30);

	controls->Add(std::make_shared<WidgetBitmapButton>(280, 125, play_button_function, tButtons, 0), 500, 200);
	controls->Add(std::make_shared<WidgetBitmapButton>(280, 125, [](void*) {Quit(); }, tButtons, 1), 500, 500);

	gui.Add(controls, 0, 0);
}

TitleScreen::~TitleScreen()
{
}

void TitleScreen::Render()
{
	gui.Draw(0, 0);
}

void TitleScreen::Update(float deltatime)
{
	/*fPeriod += deltatime;
	fPeriod = fmod(fPeriod, 2.0f);
	float size = sin(fPeriod * M_PI);
	
	float width = 480 + size * 50.0f;
	logo->width = width;
	logo->x = 640 - width / 2;*/
	
	int mouse_x, mouse_y;
	GetRelativeMousePos(&mouse_x, &mouse_y);
	gui.Update(deltatime, mouse_x, mouse_y);
}

void TitleScreen::LeftClick()
{
	int nCursorX, nCursorY;
	GetRelativeMousePos(&nCursorX, &nCursorY);
	gui.ClickOn(nCursorX, nCursorY, this);
}
