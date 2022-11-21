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
	SpaceGame::Instance().GetPlayer().fEnergy += 15 * deltatime;
	if (SpaceGame::Instance().GetPlayer().fEnergy > SpaceGame::Instance().GetPlayer().fMaxEnergy)
		SpaceGame::Instance().GetPlayer().fEnergy = SpaceGame::Instance().GetPlayer().fMaxEnergy;
}

EnergyPowerupItem::EnergyPowerupItem(Entity* owner)
	: Item(owner)
{
	texture = Textures::EnergyPowerup;
	nCount = 1;
	nType = Type::EnergyPowerup;
	strName = "Energy Powerup";
}

bool EnergyPowerupItem::Use(float fX, float fY, float fAngle)
{
	if (SpaceGame::Instance().GetPlayer().puCurrentPowerup == nullptr)
	{
		nCount--;
		SpaceGame::Instance().GetPlayer().puCurrentPowerup = std::make_shared<EnergyPowerup>();
	}
	return false;
}
