#pragma once
#include "Texture.h"
#include "Space.h"
#include "Item.h"

class Weapon : public Item
{
public:
	Weapon(const std::shared_ptr<Entity>& owner)
		:Item(owner) {}
};