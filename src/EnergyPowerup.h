#pragma once
#include "Powerup.h"
#include "Space.h"

class EnergyPowerup : public Powerup
{
public:
	EnergyPowerup();
	void Update(float deltatime) override;
};

class EnergyPowerupItem : public Item
{
public:
	EnergyPowerupItem(Entity* owner);
	bool Use(float fX, float fY, float fAngle) override;
};