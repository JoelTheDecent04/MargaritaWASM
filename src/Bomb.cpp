#include "Bomb.h"
#include "Space.h"
#include "Player.h"
#include "BombAnimation.h"

Bomb::Bomb(float fX, float fY, float fSpeedX, float fSpeedY, int nLevel)
	: Entity(Textures::Bomb, fX, fY, Type::Bomb, Category::PROJECTILE)
{
	this->fSpeedX = fSpeedX;
	this->fSpeedY = fSpeedY;
	nCollisionCategories = Category::ENEMY | Category::STRUCTURE;
	bIsBullet = true;
	fAge = 0.0f;
	this->nLevel = nLevel;
}

bool Bomb::Collide(Entity& entity)
{
	Explode();
	return false;
}

void Bomb::Explode()
{
	SpaceGame::Instance().BackgroundObjects.Add(new BombAnimation(this));
	for (auto& entity : SpaceGame::Instance().Entities)
	{
		if (&entity == this || entity.bIsBullet) continue;
		float fDistance = Distance(entity);
		float fDamageDistance = 75.0f + 25.0f * nLevel;
		if (fDistance < fDamageDistance)
			entity.ChangeHealth((-250.0f - 20.0f * nLevel) * ((fDamageDistance - fDistance) / fDamageDistance), this);
	}

	fHealth = 0.0f;
}


Entity::Status Bomb::Update(float deltatime)
{
	if (Entity::Update(deltatime) == Status::REMOVE) return Status::REMOVE;
	fAge += deltatime;
	if (fAge >= 0.75f + 0.25f * nLevel)
	{
		Explode();
		return Status::REMOVE;
	}
	return Status::KEEPALIVE;
}

BombWeapon::BombWeapon(Entity* owner, int nLevel)
	: Weapon(owner)
{
	this->nLevel = nLevel;
	nCount = nLevel;
	texture = Textures::Bomb;
	nType = Type::Bomb;
	strName = "Bomb";
}

bool BombWeapon::Use(float fX, float fY, float fAngle)
{
	if (SpaceGame::Instance().GetPlayer().fEnergy >= 8.0f + 3.0f * nLevel)
	{
		SpaceGame::Instance().Entities.Add(new Bomb(fX, fY, 300.0f * cos(fAngle), 300.0f * sin(fAngle), nLevel));
		SpaceGame::Instance().GetPlayer().fEnergy -= 8.0f + 3.0f * nLevel;
	}
	return true;
}

List<ItemUpgrade> BombWeapon::GetPossibleUpgrades()
{
	List<ItemUpgrade> upgrades;

	if (nLevel < SpaceGame::Instance().Wave())
		upgrades.Add(new ItemUpgrade(500, nLevel, "Bomb Explosion", Textures::Bomb));

	return upgrades;
}
