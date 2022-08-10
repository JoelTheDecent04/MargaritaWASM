#pragma once
#include "Entity.h"

class Enemy : public Entity
{
	float fSecondsUntilNextAttack;
	bool bShoot;

public:
	bool bLegalPosition;
	Enemy(float fX, float fY);
	bool Update(float deltatime) override;
	void OnDestroy() override;
	bool IsEnemy() override { return true; }
};