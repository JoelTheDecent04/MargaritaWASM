#include "AirStrike.h"
#include "EventHandler.h"
#include "Bomb.h"
#include "Player.h"

AirStrikeItem::AirStrikeItem(Entity* owner)
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
		auto* airstrike = new AirStrike(fX, fY, 500.0f * cos(fAngle), 500.0f * sin(fAngle));
		SpaceGame::Instance().Entities.Add(airstrike);
		nCount--;

		if (player)
			player->fMoney -= 50.0f;
	}
	return nCount == 0;
}

AirStrike::AirStrike(float fX, float fY, float fSpeedX, float fSpeedY)
	: Entity(nullptr, fX, fY, Type::Airstrike, Category::PROJECTILE)
{
	this->fSpeedX = fSpeedX;
	this->fSpeedY = fSpeedY;

	nCollisionCategories = Category::NONE;
	bIsBullet = true;
}

Entity::Status AirStrike::Update(float deltatime)
{
	if (launched)
		return Status::REMOVE;

	return Entity::Update(deltatime);
}

void AirStrike::LandOnGround()
{
	SpaceGame::Instance().Entities.Add(new Bomb(fX - 200.0f, -50.0f, 0.0f, 220.0f, 3));
	SpaceGame::Instance().Entities.Add(new Bomb(fX		    , -50.0f, 0.0f, 220.0f, 3));
	SpaceGame::Instance().Entities.Add(new Bomb(fX + 200.0f, -50.0f, 0.0f, 220.0f, 3));
	launched = true;
}
