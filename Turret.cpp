#include "Turret.h"
#include "Space.h"
#include "Laser.h"
#include "Bomb.h"

Turret::_nearest_entity Turret::FindNearestEnemy()
{
	using namespace Game;
	_nearest_entity nearest_player = { nullptr, INFINITY };
	for (auto* entity : SpaceGame::Instance().Entities()) {
		float dist;
		if ((entity->GetType() == Entity::Type::Crab || entity->GetType() == Entity::Type::Enemy) && (dist = Distance(entity)) < nearest_player.distance) {
			nearest_player.entity = entity;
			nearest_player.distance = dist;
		}
	}
	return nearest_player;
}

Turret::Turret(float x, float y)
	: Entity(TextureID::None, x, y)
{
	time_until_next_fire = 1.0f;
	bCanCollide = false;
	bCanCollideWithPlayer = false;
	bAffectedByGravity = true;
}

bool Turret::Update(float deltatime)
{
	time_until_next_fire -= deltatime;
	if (time_until_next_fire <= 0.0f) {
		time_until_next_fire += 1.0f;
		
		auto nearest_enemy = FindNearestEnemy();
		if (nearest_enemy.entity && nearest_enemy.distance < 500.0f) {
			float fGradient = (fY - nearest_enemy.entity->fY) / (fX - nearest_enemy.entity->fX);
			float fAngle = atan(fGradient);
			if (nearest_enemy.entity->fX < fX) 
				fAngle += 3.1415926f;

			weapon->Use(fX, fY, fAngle);
		}
	}
	return Entity::Update(deltatime);
}

TurretPlacer::TurretPlacer(Entity* owner)
	: Item(owner)
{
	nType = Type::TurretPlacer;
	nCount = 5;
	strName = "Turret";
	nTexture = TextureID::None;
}

bool TurretPlacer::Use(float fX, float fY, float)
{
	auto* turret = new Turret(fX, fY);
	//turret->SetWeapon(std::make_shared<LaserWeapon>(turret, LaserWeapon::LaserLevel::DoubleShot));
	turret->SetWeapon(std::make_shared<LaserWeapon>(turret, LaserWeapon::LaserLevel::DoubleShot));
	SpaceGame::Instance().AddEntity(turret);

	nCount--;
	return nCount == 0;
}
