#include "Enemy.h"
#include "Space.h"
#include "Player.h"
#include "Utilities.h"
#include "Particle.h"
#include "Coin.h"
#include "Laser.h"

Enemy::Enemy(float fX, float fY)
	: Entity(TextureID::Enemy, fX, fY)
{
	fHealth = 100.0f;
	fMaxHealth = 100.0f;
	bAffectedByGravity = false;
	fWidth = 32.0f;
	fHeight = 49.0f;
	fSecondsUntilNextAttack = 0.0f;
	bLegalPosition = true;

	for (auto* entity : SpaceGame::Instance().Entities())
	{
		if (Overlapping(entity))
		{
			bLegalPosition = false;
			break;
		}
	}

	nType = Type::Enemy;
	bShoot = randomf() < 0.075f;
	if (bShoot) nFrame = 1;
}

Entity::Status Enemy::Update(float deltatime)
{
	if (fHealth == 0.0f) {
		OnDestroy();
		return Status::REMOVE;
	}

	float dist_to_player = Distance(SpaceGame::Instance().GetPlayer());
	auto player = SpaceGame::Instance().GetPlayer();

	if (dist_to_player > 55.0f)
	{
		float fGradient = (player->fY - fY) / (player->fX - fX);
		float fAngle = atan(fGradient);
		if (player->fX < fX) fAngle += 3.1415926f;

		fSpeedX = cos(fAngle) * (100.0f + SpaceGame::Instance().Difficulty() / 60.0f);
		fSpeedY = sin(fAngle) * (100.0f + SpaceGame::Instance().Difficulty() / 60.0f);
	}
	else
	{
		fSpeedX = 0.0f;
		fSpeedY = 0.0f;
	}

	if (fSecondsUntilNextAttack > 0.0f)
		fSecondsUntilNextAttack -= deltatime;
	else
	{
		if (dist_to_player <= 55.0f && !bShoot)
			SpaceGame::Instance().GetPlayer()->ChangeHealth(-10.0f, this);
		
		if (dist_to_player < 400.0f && bShoot)
			SpaceGame::Instance().AddEntity(new LaserBeam(fX, fY, 1000.f * (player->fX - fX) / dist_to_player, 1000.f * (player->fY - fY) / dist_to_player, LaserBeam::LaserType::DamagePlayers));

		fSecondsUntilNextAttack = 1.0f;
	}

	return Entity::Update(deltatime);
}

void Enemy::OnDestroy()
{
	//SpaceGame::Instance().Player()->fMoney += 10.0f;

	if (randomf() <= 0.1f) {
		SpaceGame::Instance().AddEntity(new CoinReward(fX, fY));
		SpaceGame::Instance().GetPlayer()->fMoney += 50.f;
	}

	SpaceGame::Instance().EnemyCount()--;

	Particle::CreateParticles(fX, fY, clrDarkRed);
}