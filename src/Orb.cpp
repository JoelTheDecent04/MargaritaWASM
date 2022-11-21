#include "Orb.h"
#include "Space.h"
#include "Player.h"

Orb::Orb(float fX, float fY, float fSpeedX, float fSpeedY)
	: Entity(Textures::Orb, fX, fY, Type::Orb, Category::PROJECTILE)
{
	this->fSpeedX = fSpeedX;
	this->fSpeedY = fSpeedY;
	nCollisionCategories = Category::ENEMY | Category::STRUCTURE;
	bIsBullet = true;
	fHealth = 100.0f;
	fTimeSinceLastHit = 0.0f;

	fAge = 0.0f;
	nType = Type::Orb;
	bCanPenetrate = true;
}

Entity::Status Orb::Update(float deltatime)
{
	fAge += deltatime;
	fTimeSinceLastHit += deltatime;

	if (fAge >= 1.0f && Distance(SpaceGame::Instance().GetPlayer()) < 40.0f)
	{
		OnDestroy();
		SpaceGame::Instance().GetItem("Orb")->nCount++;
		return Status::REMOVE;
	}

	nCollisionCategories = (fSpeedX != 0.0f || fSpeedY != 0.0f) ? Category::ENEMY | Category::STRUCTURE | Category::NONE : Category::NONE ;

	Entity::Update(deltatime);

	if (fX < 0.0f)
		fX = 0.0f;
	else if (fX > 5120.0f)
		fX = 5120.0f;

	return Status::KEEPALIVE;
}

bool Orb::Collide(Entity& entity)
{
	if (fTimeSinceLastHit > 0.35f)
	{
		entity.ChangeHealth(-100.0f, this);
		fSpeedX *= 0.8f;
		fSpeedY *= 0.8f;
		fTimeSinceLastHit = 0.0f;
	}
	return true;
}

OrbWeapon::OrbWeapon(Entity* owner)
	: Weapon(owner)
{
	nCount = 10;
	texture = Textures::Orb;
	nType = Type::Orb;
	strName = "Orb";
}

bool OrbWeapon::Use(float fX, float fY, float fAngle)
{
	if (nCount)
	{
		SpaceGame::Instance().Entities.Add(new Orb(fX, fY, 300.0f * cos(fAngle), 300.0f * sin(fAngle)));
		nCount--;
	}
	return true;
}

List<ItemUpgrade> OrbWeapon::GetPossibleUpgrades()
{
	List<ItemUpgrade> upgrades;
	upgrades.Add(new ItemUpgrade(250, this->nCount, "Extra Orbs", Textures::Orb, 0, 2));
	return upgrades;
}