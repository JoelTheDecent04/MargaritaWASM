#pragma once
#include "Entity.h"
#include "Animation.h"

class Crab : public Entity
{
	float fSecondsUntilNextAttack;
	Animation animation;
	
public:
	bool bLegalPosition;
	Crab(float fX);
	bool Update(float deltatime) override;
	void OnDestroy() override;
	bool IsEnemy() override { return true; }
};