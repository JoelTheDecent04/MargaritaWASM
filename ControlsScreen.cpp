#include "ControlsScreen.h"
#include "PauseScreen.h"
#include "Graphics.h"
#include "Player.h"
#include "Utilities.h"
#include "Colours.h"
#include "ControlManager.h"

using namespace Game;

class ControlChanger : public WidgetGroup
{
	class ControlChangerButton : public WidgetButton
	{
		ControlChanger* parent;
		bool secondary;
	public:
		ControlChangerButton(ControlChanger* parent, bool secondary)
			: WidgetButton(100, 28, nullptr, "")
		{
			this->parent = parent;
			this->secondary = secondary;
		}
		void ClickOn(int, int, void*) override
		{
			parent->SetReadyToChange(secondary);
		}
	};

	std::shared_ptr<ControlChangerButton> primary;
	std::shared_ptr<ControlChangerButton> secondary;

	std::string primary_button_text;
	std::string secondary_button_text;

	Control& control;

	bool changing = false;
	bool changing_secondary = false;

public:
	ControlChanger(Control& control)
		: control(control),
		WidgetGroup(410, 28)
	{
		primary = std::make_shared<ControlChangerButton>(this, false);
		secondary = std::make_shared<ControlChangerButton>(this, true);
	
		Add(primary, 200, 0);
		Add(secondary, 310, 0);
		Add(std::make_shared<WidgetLabel>(control.Description(), Alignment::LeftCentre), 0, 14);
	}
	void Update(float deltatime, int mouse_x, int mouse_y) override
	{
		primary_button_text = ControlManager::KeyText(control.Primary());
		secondary_button_text = ControlManager::KeyText(control.Secondary());

		if (changing && !changing_secondary) {
			primary_button_text.insert(0, 1, '[');
			primary_button_text.append(1, ']');
		}
		else if (changing && changing_secondary) {
			secondary_button_text.insert(0, 1, '[');
			secondary_button_text.append(1, ']');
		}

		primary->SetText(primary_button_text.c_str());
		secondary->SetText(secondary_button_text.c_str());
		WidgetGroup::Update(deltatime, mouse_x, mouse_y);
	}
	void SetReadyToChange(bool secondary)
	{
		changing = true;
		changing_secondary = secondary;
	}
	void KeyDown(int key) override
	{
		if (changing && !changing_secondary)
			control.SetPrimary(key);
		else if (changing && changing_secondary)
			control.SetSecondary(key);

		changing = false;
	}
	void ClickOff(int, int, void*) override
	{
		changing = false;
	}
};

ControlsScreen::ControlsScreen()
	: gui(1280, 720)
{
	this->lPreviousLevel = Game::lCurrentLevel;

	gui.Add(std::make_shared<WidgetLabel>("Controls", Alignment::Centre, 44), 640, 50);

	for (int i = 0; i < ControlManager::Instance().AllControls().size(); i++)
	{
		gui.Add(std::make_shared<ControlChanger>(ControlManager::Instance().AllControls()[i]), 200, 100 + 40 * i);
	}
}

ControlsScreen::~ControlsScreen()
{
}

void ControlsScreen::Render()
{
	Graphics::Clear(clrBlack);
	gui.Draw(0, 0);
}

void ControlsScreen::Update(float deltatime)
{
	int nCursorX, nCursorY;
	GetRelativeMousePos(&nCursorX, &nCursorY);
	gui.Update(deltatime, nCursorX, nCursorY);

	

	bool bMouseOverButton = false;
	int i = 0;
	for (Button& button : vButtons)
	{
		if (PointInRect(button.rect, nCursorX, nCursorY))
		{
			nButtonHover = i;
			bMouseOverButton = true;
			break;
		}
		i++;
	}
	if (!bMouseOverButton) nButtonHover = -1;
}

void ControlsScreen::LeftClick()
{
	int nCursorX, nCursorY;
	GetRelativeMousePos(&nCursorX, &nCursorY);
	gui.ClickOn(nCursorX, nCursorY, this);
}

void ControlsScreen::KeyDown(int key)
{
	if (key == SDL_SCANCODE_ESCAPE)
	{
		LoadLevel(lPreviousLevel);
		return;
	}

	gui.KeyDown(key);
}
