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
	float nEnergy;
	float nMaxEnergy;
	float fEnergyRechargeSpeed;
	float fMaxEnergyRechargeSpeed;
	float fMaxHealthUpgrade;
	float fMovementSpeed;
	float fMaxMovementSpeed;
	float fHealthRegeneration;
	float fMaxHealthRegeneration;
	Player(float fX, float fY);
	Status Update(float deltatime) override;
	void OnDestroy() override;

	std::vector<std::unique_ptr<PlayerUpgrade>> GetPossibleUpgrades();
};

extern int keyJump1, keyJump2, keyMoveLeft1, keyMoveLeft2, keyMoveRight1, keyMoveRight2, keyMoveDown1, keyMoveDown2;