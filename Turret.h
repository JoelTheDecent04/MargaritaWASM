#include "Entity.h"
#include "Weapon.h"

class Turret : public Entity
{
	std::shared_ptr<Weapon> weapon;
	float time_until_next_fire;

	struct _nearest_entity {
		Entity* entity; 
		float distance; 
	};
	_nearest_entity FindNearestEnemy();
public:
	Turret(float x, float y);
	void SetWeapon(const std::shared_ptr<Weapon>& weapon) {
		this->weapon = weapon;
	}
	bool Update(float deltatime) override;
};

class TurretPlacer : public Item
{
public:
	TurretPlacer(Entity* owner);
	bool Use(float fX, float fY, float fAngle) override;
};