#pragma once
#include "Powerup.h"
#include "Space.h"

class RegenerationPowerup : public Powerup
{
public:
	RegenerationPowerup();
	void Update(float deltatime) override;
};


class RegenerationPowerupItem : public Item
{
public:
	RegenerationPowerupItem(Entity* owner);
	bool Use(float fX, float fY, float fAngle) override;
};