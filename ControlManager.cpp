#include "ControlManager.h"
#include "Utilities.h"
#include <SDL.h>

ControlManager ControlManager::control_manager;

Control unknown_control = Control(Control::Action::None, "Unknown", 0, 0);

ControlManager::ControlManager()
{
	AddControl(Control::Action::Jump, "Jump", SDL_SCANCODE_W, SDL_SCANCODE_SPACE);
	AddControl(Control::Action::Left, "Move Left", SDL_SCANCODE_A);
	AddControl(Control::Action::Right, "Move Right", SDL_SCANCODE_D);
	AddControl(Control::Action::Down, "Move Down", SDL_SCANCODE_S);

	AddControl(Control::Action::OpenShop, "Open Shop", SDL_SCANCODE_E);
	AddControl(Control::Action::NextWave, "Next Wave", SDL_SCANCODE_R);

	AddControl(Control::Action::ShowHideMap, "Show/Hide Map", SDL_SCANCODE_M);

	AddControl(Control::Action::Slot1, "Slot 1", SDL_SCANCODE_1);
	AddControl(Control::Action::Slot2, "Slot 2", SDL_SCANCODE_2);
	AddControl(Control::Action::Slot3, "Slot 3", SDL_SCANCODE_3);
	AddControl(Control::Action::Slot4, "Slot 4", SDL_SCANCODE_4);
	AddControl(Control::Action::Slot5, "Slot 5", SDL_SCANCODE_5);
	AddControl(Control::Action::Slot6, "Slot 6", SDL_SCANCODE_6);
	AddControl(Control::Action::Slot7, "Slot 7", SDL_SCANCODE_7);
	AddControl(Control::Action::Slot8, "Slot 8", SDL_SCANCODE_8);
}

const Control& ControlManager::GetControl(int key)
{
	for (auto& control : controls) {
		if (key == control.Primary())
			return control;
	}
	for (auto& control : controls) {
		if (key == control.Secondary())
			return control;
	}

	return unknown_control;
}

const Control& ControlManager::GetControlFromAction(Control::Action action)
{
	for (auto& control : controls) {
		if (action == control.GetAction())
			return control;
	}
	assert_not_reached();
}

bool ControlManager::IsControlActivated(const Control& control)
{
	return KeyState(control.Primary()) || KeyState(control.Secondary());
}

void ControlManager::AddControl(Control::Action action, const char* description, int primary, int secondary)
{
	controls.push_back(Control(action, description, primary, secondary));
}

const char* ControlManager::KeyText(int nKey)
{
	return SDL_GetKeyName(SDL_GetKeyFromScancode((SDL_Scancode)nKey));
}


Control::Control(Action action, const char* description, int primary, int secondary)
{
	this->action = action;
	this->description = description;
	this->primary = primary;
	this->secondary = secondary;
}

const char* Control::PrimaryKeyText() const
{
	return ControlManager::KeyText(primary);
}
