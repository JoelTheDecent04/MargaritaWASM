#pragma once
#include "Texture.h"
#include "Space.h"
#include "Upgrades.h"
#include <fstream>

class Item : public std::enable_shared_from_this<Item>
{
protected:
	std::weak_ptr<Entity> owner;

public:
	enum class Type { None, Bomb, Laser, Orb, EnergyPowerup, RegenerationPowerup, TurretPlacer, Block };
	Type nType;
	int nCount;
	const char* strName;
	int nTexture;
	int nTextureFrame = 0;

	virtual std::vector<std::unique_ptr<ItemUpgrade>> GetPossibleUpgrades() { return {}; };

	virtual bool Use(float fX, float fY, float fAngle) = 0;
	virtual void HoldUpdate() {};
	virtual void HoldRender() {};
	Item(const std::shared_ptr<Entity>& owner);
	std::weak_ptr<Entity> Owner() {
		return owner;
	}
	const char* Name() { return strName; }
	Entity* GetOwner() {
		std::shared_ptr<Entity> the_owner = owner.lock();
		if (the_owner)
			return the_owner.get();
		else
			return nullptr;
	}
};