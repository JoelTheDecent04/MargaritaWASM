#include "Item.h"
#include "Entity.h"

class AirStrikeItem : public Item
{
public:
	AirStrikeItem(Entity* owner);
	bool Use(float fX, float fY, float fAngle) override;
};

class AirStrike : public Entity
{
	bool launched = false;
public:
	AirStrike(float fX, float fY, float fSpeedX, float fSpeedY);

	Status Update(float deltatime) override;
	void LandOnGround() override;
};