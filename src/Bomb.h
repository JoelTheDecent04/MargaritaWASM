#pragma once
#include "Entity.h"
#include "Weapon.h"

class Bomb : public Entity
{
	float fAge;
public:
	int nLevel;
	Bomb(float fX, float fY, float fSpeedX, float fSpeedY, int nLevel);
	bool Collide(Entity& entity) override;
	void Explode();
	Status Update(float deltatime) override;

};

class BombWeapon : public Weapon
{
public:
	int nLevel;
	BombWeapon(Entity* owner, int nLevel);
	bool Use(float fX, float fY, float fAngle) override;
	List<ItemUpgrade> GetPossibleUpgrades() override;
	void Upgrade() {
		nLevel++;
		nCount = nLevel;
	}
};