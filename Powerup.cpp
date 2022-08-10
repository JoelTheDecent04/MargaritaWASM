#include "Powerup.h"
#include "Player.h"


void Powerup::Remove()
{
	SpaceGame::Instance().Player()->puCurrentPowerup = nullptr;
}
