#pragma once
#include "Texture.h"
#include "Space.h"
#include "Item.h"

class Weapon : public Item
{
public:
	Weapon(Entity* owner)
		:Item(owner) {}
};