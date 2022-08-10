#pragma once
#include "Item.h"

class Powerup
{
public:
	float fTimeLeft;

	Powerup() { fTimeLeft = 15.0f; };

	virtual void Update(float deltatime) = 0;
	void Remove();
};