#pragma once
#include "Entity.h"

class Snail : public Entity
{
    float fSecondsUntilNextAttack;
    bool pathfinding;
public:
    Snail();
	Status Update(float deltatime) override;
    bool ShouldCollideWith(Entity& entity) override;
};