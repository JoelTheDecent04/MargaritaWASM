#include "Crab.h"
#include "Space.h"
#include "Player.h"
#include "Particle.h"

Crab::Crab(float fX)
	: Entity(Textures::Crab, fX, 594 - (48 / 2), Type::Crab, Category::ENEMY),
	animation(Textures::Character, 14, 60.0f, &nFrame) //TODO wtf?
{
	nCollisionCategories = Category::ENEMY | Category::PLAYER | Category::STRUCTURE;

	fHealth = 200.0f;
	fMaxHealth = 200.0f;
	bAffectedByGravity = false;
	fWidth = 88.0f;
	fHeight = 44.0f;
	fSecondsUntilNextAttack = 0.0f;
	bLegalPosition = true;

	for (auto& entity : SpaceGame::Instance().Entities)
	{
		if (Overlapping(entity))
		{
			bLegalPosition = false;
			break;
		}
	}
}
Entity::Status Crab::Update(float deltatime)
{
	if (abs(SpaceGame::Instance().GetPlayer().fX - fX) > 80.0f)
		fSpeedX = SpaceGame::Instance().GetPlayer().fX > fX ? 100.0f + SpaceGame::Instance().Difficulty() / 60.0f : -100.0f - SpaceGame::Instance().Difficulty() / 60.0f;
	else
	{
		fSpeedX = 0.0f;
		if (fSecondsUntilNextAttack > 0.0f)
			fSecondsUntilNextAttack -= deltatime;
		else if (Distance(SpaceGame::Instance().GetPlayer()) <= 90.0f)
		{
			SpaceGame::Instance().GetPlayer().ChangeHealth(-15.0f, this);
			fSecondsUntilNextAttack = 1.0f;
		}
	}

	animation.Update(deltatime);
	
 	if (Entity::Update(deltatime) == Status::REMOVE) 
		return Status::REMOVE;

	if (abs(fSpeedX) > 0.0f)
		animation.Start();
	else
		animation.Stop();

	return Status::KEEPALIVE;
}

void Crab::OnDestroy()
{
	SpaceGame::Instance().GetPlayer().fMoney += 15.0f;
	SpaceGame::Instance().EnemyCount()--;

	Particle::CreateParticles(fX, fY, clrPurple);
}