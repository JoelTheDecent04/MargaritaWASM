#include "Bomb.h"
#include "Space.h"
#include "Player.h"
#include "BombAnimation.h"

Bomb::Bomb(float fX, float fY, float fSpeedX, float fSpeedY, int nLevel)
	: Entity(TextureID::Bomb, fX, fY)
{
	this->fSpeedX = fSpeedX;
	this->fSpeedY = fSpeedY;
	bCanCollideWithPlayer = false;
	bIsBullet = true;
	fAge = 0.0f;
	this->nLevel = nLevel;

	nType = Type::Bomb;
}

bool Bomb::Collide(Entity* entity)
{
	Explode();
	return false;
}

void Bomb::Explode()
{
	SpaceGame::Instance().AddBackgroundObject(std::make_shared<BombAnimation>(this));
	for (auto& entity : SpaceGame::Instance().Entities())
	{
		if (entity.get() == this || entity->bIsBullet) continue;
		float fDistance = Distance(entity.get());
		float fDamageDistance = 75.0f + 25.0f * nLevel;
		if (fDistance < fDamageDistance)
			entity->ChangeHealth((-250.0f - 20.0f * nLevel) * ((fDamageDistance - fDistance) / fDamageDistance), this);
	}

	fHealth = 0.0f;
}


bool Bomb::Update(float deltatime)
{
	if (Entity::Update(deltatime) == false) return false;
	fAge += deltatime;
	if (fAge >= 0.75f + 0.25f * nLevel)
	{
		Explode();
		return false;
	}
	return true;
}

BombWeapon::BombWeapon(const std::shared_ptr<Entity>& owner, int nLevel)
	: Weapon(owner)
{
	this->nLevel = nLevel;
	nCount = nLevel;
	nTexture = TextureID::Bomb;
	nType = Type::Bomb;
	strName = "Bomb";
}

bool BombWeapon::Use(float fX, float fY, float fAngle)
{
	if (SpaceGame::Instance().Player()->nEnergy >= 8.0f + 3.0f * nLevel)
	{
		SpaceGame::Instance().AddEntity(std::make_shared<Bomb>(fX, fY, 300.0f * cos(fAngle), 300.0f * sin(fAngle), nLevel));
		SpaceGame::Instance().Player()->nEnergy -= 8.0f + 3.0f * nLevel;
	}
	return true;
}

std::vector<std::unique_ptr<ItemUpgrade>> BombWeapon::GetPossibleUpgrades()
{
	std::vector<std::unique_ptr<ItemUpgrade>> upgrades;

	upgrades.push_back(std::make_unique<ItemUpgrade>(500, nLevel, "Bomb Explosion"));

	return upgrades;
}
