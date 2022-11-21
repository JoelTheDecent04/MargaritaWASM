#pragma once
#include "Texture.h"
#include "Space.h"
#include "Upgrades.h"
#include <fstream>

class Item : public std::enable_shared_from_this<Item>
{
protected:
	Entity* owner;

public:
	enum class Type { None, Bomb, Laser, Orb, EnergyPowerup, RegenerationPowerup, TurretPlacer, Block };
	Type nType;
	int nCount;
	const char* strName;
	Texture* texture;
	int nTextureFrame = 0;

	virtual List<ItemUpgrade> GetPossibleUpgrades() { return {}; };

	virtual bool Use(float fX, float fY, float fAngle) = 0;
	virtual void HoldUpdate() {};
	virtual void HoldRender() {};
	Item(Entity* owner);
	Entity* Owner() {
		return owner;
	}
	const char* Name() const { return strName; }
	Entity* GetOwner() {
		return owner;
	}
	virtual ~Item() {}
};