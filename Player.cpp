#include "Player.h"
#include "Space.h"
#include "DeathScreen.h"
#include "Bomb.h"
#include "ControlManager.h"

static uint32_t nLastCharacterId = 0;

int keyJump1 = SDL_SCANCODE_W;
int keyJump2 = SDL_SCANCODE_SPACE;
int keyMoveLeft1 = SDL_SCANCODE_A;
int keyMoveLeft2 = 0;
int keyMoveRight1 = SDL_SCANCODE_D;
int keyMoveRight2 = 0;
int keyMoveDown1 = SDL_SCANCODE_S;
int keyMoveDown2 = 0;

Player::Player(float fX, float fY)
	: Entity(TextureID::Character, fX, fY)
{
	fHealth = 200.0f;
	fMaxHealth = 200.0f;
	nEnergy = 150.0f;
	nMaxEnergy = 150.0f;
	fMoney = 0.0f;
	fEnergyRechargeSpeed = 3.0f;
	fMaxEnergyRechargeSpeed = 9.0f;
	fMaxHealthUpgrade = 450.0f;
	fWidth = 50.0f;
	fHeight = 58.0f;
	fMovementSpeed = 150.0f;
	fMaxMovementSpeed = 190.0f;
	puCurrentPowerup = nullptr;
	fHealthRegeneration = 2.0f;
	fMaxHealthRegeneration = 5.0f;

	nType = Type::Player;
}

Entity::Status Player::Update(float deltatime)
{
	if (fHealth <= 0.0f)
	{
		OnDestroy();
		return Status::REMOVE; 
	}
	
	static const auto& move_left = ControlManager::Instance().GetControlFromAction(Control::Action::Left);
	if (ControlManager::IsControlActivated(move_left))
	{
		fSpeedX -= fPlayerAcceleration * deltatime;
		if (fSpeedX < -fMovementSpeed)
			fSpeedX = -fMovementSpeed;
	}
	static const auto& move_right = ControlManager::Instance().GetControlFromAction(Control::Action::Right);
	if (ControlManager::IsControlActivated(move_right))
	{
		fSpeedX += fPlayerAcceleration * deltatime;
		if (fSpeedX > fMovementSpeed)
			fSpeedX = fMovementSpeed;
	}
	
	static const auto& move_down = ControlManager::Instance().GetControlFromAction(Control::Action::Down);
	if (ControlManager::IsControlActivated(move_down))
		fSpeedY += fPlayerMoveDownSpeed * deltatime;

	static const auto& move_up = ControlManager::Instance().GetControlFromAction(Control::Action::Jump);
	if (ControlManager::IsControlActivated(move_up))
	{
		fSpeedY = -180.0f;
		bOnGround = false;
	}

	if (fSpeedX > 0)
	{
		fSpeedX -= fPlayerDecceleration * deltatime;
		if (fSpeedX < 0)
			fSpeedX = 0;
	}
	if (fSpeedX < 0)
	{
		fSpeedX += fPlayerDecceleration * deltatime;
		if (fSpeedX > 0)
			fSpeedX = 0;
	}

	if (!bOnGround)
		fSpeedY += fGravity * deltatime;

//-------------------------MOVEMENT-----------------------------------------

	bool bCollided = false;

	float fNewX = fX + fSpeedX * deltatime; //Get new position
	float fNewY = fY + fSpeedY * deltatime;

 	bool bCollidedVertically = false;
	for (auto* entity : SpaceGame::Instance().Entities()) //Check if it can move vertically
	{
		if (entity == this) continue;
		if (!entity->bCanCollideWithPlayer) continue;
		if (entity->WillOverlap(this, fX, fNewY))
		{
			bCollidedVertically = true;
			bCollided = true;
			Collide(entity);
			break;
		}
	}
	if (bCollidedVertically)
		fSpeedY = 0.0f;
	else
		fY = fNewY;

	bool bCollidedHorizontally = false;
	for (auto* entity : SpaceGame::Instance().Entities()) //Check if it can move horizontally
	{
		if (entity == this) continue;
		if (!entity->bCanCollideWithPlayer) continue;
		if (entity->WillOverlap(this, fNewX, fY))
		{
			bCollidedHorizontally = true;
			if (!bCollided)
				Collide(entity);
			break;
		}
	}
	if (bCollidedHorizontally)
		fSpeedX = 0.0f;
	else
		fX = fNewX;



	if (fX < 0)
		fX = 0;
	if (fX > 5120)
		fX = 5120;
	if (fY <= 0.0f)
	{
		fY = 0.0f;
		fSpeedY = 0.0f;
	}
	if (fY >= 594.0f - fHeight / 2)
	{
		fY = 594.0f - fHeight / 2;
		fSpeedY = 0.0f;
		bOnGround = true;
	}

	nEnergy += (bOnGround ? fEnergyRechargeSpeed : fEnergyRechargeSpeed / 2.0f) * deltatime;
	if (nEnergy > nMaxEnergy) nEnergy = nMaxEnergy;
	fHealth += fHealthRegeneration * deltatime;
	if (fHealth > fMaxHealth) fHealth = fMaxHealth;

	if (fMoney >= 150.0f && SpaceGame::Instance().GetItem("Bomb") == nullptr)
		SpaceGame::Instance().AddItem(new BombWeapon(this, 1));

	if (puCurrentPowerup)
	{
		puCurrentPowerup->Update(deltatime);
		nFrame = 1;
	}
	else
		nFrame = 0;

	return Status::KEEPALIVE;
}

void Player::OnDestroy()
{
	SpaceGame::Instance().PlayerCount()--;
}

std::vector<std::unique_ptr<PlayerUpgrade>> Player::GetPossibleUpgrades()
{
	std::vector<std::unique_ptr<PlayerUpgrade>> upgrades;

	if (fMovementSpeed < fMaxMovementSpeed) {
		upgrades.push_back(std::make_unique<PlayerUpgrade>(fMovementSpeed, fMaxMovementSpeed, 10.0f, 50, "Movement Speed"));
	}

	if (fHealthRegeneration < fMaxHealthRegeneration) {
		upgrades.push_back(std::make_unique<PlayerUpgrade>(fHealthRegeneration, fMaxHealthRegeneration, 10.0f, 150.0f, "Health Regeneration"));
	}

	if (fEnergyRechargeSpeed < fMaxEnergyRechargeSpeed) {
		upgrades.push_back(std::make_unique<PlayerUpgrade>(fEnergyRechargeSpeed, fMaxEnergyRechargeSpeed, 1.0f, 150, "Energy Recharge"));
	}

	if (fMaxHealth < fMaxHealthUpgrade) {
		upgrades.push_back(std::make_unique<PlayerUpgrade>(fMaxHealth, fMaxHealthUpgrade, 50.0f, 150, "Max Health"));
	}

	return upgrades;
}
