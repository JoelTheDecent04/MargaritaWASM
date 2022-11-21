#pragma once
#include "Entity.h"
#include "Space.h"
#include "Powerup.h"
#include <string>

class Player : public Entity
{
public:
	std::shared_ptr<Powerup> puCurrentPowerup;

	float fMoney;
	float fEnergy;
	float fMaxEnergy;
	float fEnergyRechargeSpeed;
	float fMaxEnergyRechargeSpeed;
	float fMaxHealthUpgrade;
	float fMovementSpeed;
	float fMaxMovementSpeed;
	float fHealthRegeneration;
	float fMaxHealthRegeneration;
	std::string Name;
	Player(float fX, float fY);
	Status Update(float deltatime) override;
	void OnDestroy() override;

	List<PlayerUpgrade> GetPossibleUpgrades();
};