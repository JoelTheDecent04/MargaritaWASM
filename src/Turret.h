#include "Entity.h"
#include "Weapon.h"

class Turret : public Entity
{
	Weapon* weapon;
	float time_until_next_fire;

	struct _nearest_entity {
		Entity* entity; 
		float distance; 
	};
	_nearest_entity FindNearestEnemy();
public:
	Turret(float x, float y);
	void SetWeapon(Weapon* weapon) {
		this->weapon = weapon;
	}
	Status Update(float deltatime) override;
	~Turret();
};

class TurretPlacer : public Item
{
public:
	TurretPlacer(Entity* owner);
	bool Use(float fX, float fY, float fAngle) override;
};