#pragma once
#include "Game.h"
#include "Space.h"
#include "GUI.h"
#include <vector>

class ShopScreen : public Level
{
	WidgetGroup gui;
	
	std::shared_ptr<WidgetGroup> player_upgrades;
	std::shared_ptr<WidgetGroup> item_upgrades;

	std::shared_ptr<WidgetButton> button_energy_powerup;
	std::shared_ptr<WidgetButton> button_regen_powerup;
	std::shared_ptr<WidgetButton> button_laser;
	std::shared_ptr<WidgetButton> button_bomb;

	void UpgradeHealth();
	void UpgradeEnergyRecharge();
	void UpgradeMovementSpeed();
	void BuyLaserUpgrade();
	void BuyEnergyPowerup();
	void BuyRegenerationPowerup();
	void BuyBombUpgrade();
	void UpgradeRegeneration();
public:
	ShopScreen();
	void Render() override;
	void Update(float deltatime) override;
	void LeftClick() override;
	void KeyDown(int key) override;
	void Resume();
	void ReloadUpgrades();

};