#pragma once
#include "Weapon.h"
#include "Entity.h"


class LaserWeapon : public Weapon
{
public:
	enum class LaserLevel {
		Normal = 1,
		DoubleShot
	};
	LaserLevel nLaserLevel;
	LaserWeapon(Entity* owner, LaserLevel nLaserLevel);
	bool Use(float fX, float fY, float fAngle) override;

	List<ItemUpgrade> GetPossibleUpgrades() override;
	void HoldUpdate() override;

	void Fire(std::weak_ptr<Entity> entity, float fAngle);

	void SetLevel(LaserLevel new_level) {
		nLaserLevel = new_level;
	}
};

class LaserBeam : public Entity
{
	float fAngle;
	
	
public:
	enum class LaserType {
		DamageEnemies,
		DamagePlayers
	};

private:
	LaserType nLaserType;
public:

	LaserBeam(float fX, float fY, float fSpeedX, float fSpeedY, LaserType nLaserType = LaserType::DamageEnemies);
	bool Collide(Entity& entity) override;
	Status Update(float deltatime) override;
	void Draw() override;
};