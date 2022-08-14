#include "RegenerationPowerup.h"
#include "Space.h"
#include "Player.h"

RegenerationPowerup::RegenerationPowerup()
	: Powerup()
{
	fTimeLeft = 10.0f;
}

void RegenerationPowerup::Update(float deltatime)
{
	fTimeLeft -= deltatime;
	if (fTimeLeft <= 0.0f)
	{
		Remove(); //Memory leak
		return;
	}

	SpaceGame::Instance().GetPlayer()->fHealth += 25 * deltatime;
	if (SpaceGame::Instance().GetPlayer()->fHealth > SpaceGame::Instance().GetPlayer()->fMaxHealth)
		SpaceGame::Instance().GetPlayer()->fHealth = SpaceGame::Instance().GetPlayer()->fMaxHealth;
}

RegenerationPowerupItem::RegenerationPowerupItem(Entity* owner)
	: Item(owner)
{
	nTexture = TextureID::RegenerationPowerup;
	nCount = 1;
	nType = Type::RegenerationPowerup;
	strName = "Regeneration Powerup";
}

bool RegenerationPowerupItem::Use(float fX, float fY, float fAngle)
{
	if (SpaceGame::Instance().GetPlayer()->puCurrentPowerup == nullptr)
	{
		nCount--;
		SpaceGame::Instance().GetPlayer()->puCurrentPowerup = std::make_shared<RegenerationPowerup>();
	}
	return false;
}
