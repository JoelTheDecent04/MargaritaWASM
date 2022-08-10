#include "AirStrike.h"
#include "EventHandler.h"
#include "Bomb.h"
#include "Player.h"

AirStrikeItem::AirStrikeItem(const std::shared_ptr<Entity>& owner)
	: Item(owner)
{
	strName = "Air Attack";
	nCount = 10;
}

bool AirStrikeItem::Use(float fX, float fY, float fAngle)
{
	auto* weapon_owner = GetOwner();
	Player* player = weapon_owner->GetType() == Entity::Type::Player ? (Player*)weapon_owner : nullptr;

	if (!player || player->fMoney > 50.0f) {
		auto airstrike = std::make_shared<AirStrike>(fX, fY, 500.0f * cos(fAngle), 500.0f * sin(fAngle));
		SpaceGame::Instance().AddEntity(airstrike);
		nCount--;

		if (player)
			player->fMoney -= 50.0f;
	}
	return nCount == 0;
}

AirStrike::AirStrike(float fX, float fY, float fSpeedX, float fSpeedY)
	: Entity(TextureID::None, fX, fY)
{
	this->fSpeedX = fSpeedX;
	this->fSpeedY = fSpeedY;

	bCanCollide = false;
	bBouncy = false;
	bIsBullet = true;
}

bool AirStrike::Update(float deltatime)
{
	if (launched)
		return false;

	return Entity::Update(deltatime);
}

void AirStrike::LandOnGround()
{
	SpaceGame::Instance().AddEntity(std::make_shared<Bomb>(fX - 200.0f, -50.0f, 0.0f, 220.0f, 3));
	SpaceGame::Instance().AddEntity(std::make_shared<Bomb>(fX		  , -50.0f, 0.0f, 220.0f, 3));
	SpaceGame::Instance().AddEntity(std::make_shared<Bomb>(fX + 200.0f, -50.0f, 0.0f, 220.0f, 3));
	launched = true;
}
