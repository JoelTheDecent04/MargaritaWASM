#pragma once
#include "Entity.h"
#include "Weapon.h"

class Orb : public Entity
{
	float fAge;
	float fTimeSinceLastHit;
public:
	Orb(float fX, float fY, float fSpeedX, float fSpeedY);
	Status Update(float deltatime) override;
	bool Collide(Entity& entity) override;
	void ChangeHealth(float, Entity*) override {};
};

class OrbWeapon : public Weapon
{
public:
	OrbWeapon(Entity* owner);
	bool Use(float fX, float fY, float fAngle) override;
	List<ItemUpgrade> GetPossibleUpgrades() override;
};