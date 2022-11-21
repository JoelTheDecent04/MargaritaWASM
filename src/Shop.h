#pragma once
#include "Game.h"
#include "Space.h"
#include "GUI.h"
#include <vector>

class ShopScreen : public Level
{
	std::shared_ptr<WidgetGroup> widget_item_list;
	std::shared_ptr<WidgetGroup> widget_player_upgrades;
	
public:
	ShopScreen();
	void render() override;
	void LeftClick() override;
	void KeyDown(int key) override;
	void Resume();
	void ReloadUpgrades();

};