#pragma once
#include "Entity.h"
#include "Weapon.h"

class Orb : public Entity
{
	float fAge;
public:
	Orb(float fX, float fY, float fSpeedX, float fSpeedY);
	bool Update(float deltatime) override;
	bool Collide(Entity* entity) override;
	void ChangeHealth(float, Entity*) override {};
	bool ShouldHit(Entity* entity) override;
};

class OrbWeapon : public Weapon
{
public:
	OrbWeapon(const std::shared_ptr<Entity>& owner);
	bool Use(float fX, float fY, float fAngle) override;
};