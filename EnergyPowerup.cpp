#include "EnergyPowerup.h"
#include "Space.h"
#include "Player.h"

EnergyPowerup::EnergyPowerup()
	: Powerup()
{
	fTimeLeft = 10.0f;
}

void EnergyPowerup::Update(float deltatime)
{
	fTimeLeft -= deltatime;
	if (fTimeLeft <= 0.0f)
	{
		Remove();
		return;
	}
	SpaceGame::Instance().Player()->nEnergy += 15 * deltatime;
	if (SpaceGame::Instance().Player()->nEnergy > SpaceGame::Instance().Player()->nMaxEnergy)
		SpaceGame::Instance().Player()->nEnergy = SpaceGame::Instance().Player()->nMaxEnergy;
}

EnergyPowerupItem::EnergyPowerupItem(const std::shared_ptr<Entity>& owner)
	: Item(owner)
{
	nTexture = TextureID::EnergyPowerup;
	nCount = 1;
	nType = Type::EnergyPowerup;
	strName = "Energy Powerup";
}

bool EnergyPowerupItem::Use(float fX, float fY, float fAngle)
{
	if (SpaceGame::Instance().Player()->puCurrentPowerup == nullptr)
	{
		nCount--;
		SpaceGame::Instance().Player()->puCurrentPowerup = std::make_shared<EnergyPowerup>();
	}
	return false;
}
