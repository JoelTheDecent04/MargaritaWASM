#include "Powerup.h"
#include "Player.h"


void Powerup::Remove()
{
	SpaceGame::Instance().GetPlayer().puCurrentPowerup = nullptr;
}
