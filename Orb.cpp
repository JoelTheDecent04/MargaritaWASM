#include "Orb.h"
#include "Space.h"
#include "Player.h"

Orb::Orb(float fX, float fY, float fSpeedX, float fSpeedY)
	: Entity(TextureID::Orb, fX, fY)
{
	this->fSpeedX = fSpeedX;
	this->fSpeedY = fSpeedY;
	bCanCollideWithPlayer = false;
	bIsBullet = true;
	fHealth = 100.0f;

	fAge = 0.0f;
	nType = Type::Orb;
}

bool Orb::Update(float deltatime)
{
	fAge += deltatime;
	if (fAge >= 1.0f && Distance(SpaceGame::Instance().Player().get()) < 40.0f)
	{
		OnDestroy();
		SpaceGame::Instance().GetItem("Orb")->nCount++;
		return false;
	}

	bCanCollide = (fSpeedX != 0.0f || fSpeedY != 0.0f);

	Entity::Update(deltatime);

	if (fX < 0.0f)
		fX = 0.0f;
	else if (fX > 5120.0f)
		fX = 5120.0f;

	return true;
}

bool Orb::Collide(Entity* entity)
{
	entity->ChangeHealth(-100.0f, this);
	fSpeedX *= 0.8f;
	fSpeedY *= 0.8f;
	return true;
}

bool Orb::ShouldHit(Entity* entity)
{
	return entity->GetType() != Type::Orb;
}

OrbWeapon::OrbWeapon(const std::shared_ptr<Entity>& owner)
	: Weapon(owner)
{
	nCount = 12;
	nTexture = TextureID::Orb;
	nType = Type::Orb;
	strName = "Orb";
}

bool OrbWeapon::Use(float fX, float fY, float fAngle)
{
	if (nCount)
	{
		SpaceGame::Instance().AddEntity(std::make_shared<Orb>(fX, fY, 300.0f * cos(fAngle), 300.0f * sin(fAngle)));
		nCount--;
	}
	return true;
}