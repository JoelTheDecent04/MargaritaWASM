#include "Entity.h"
#include "Item.h"

class Block : public Entity
{
public:
	Block(float x, float y);
	void OnDestroy() override;
};

class BlockItem : public Item
{
	int world_x = NAN;
	float world_y = NAN;
public:
	BlockItem(Entity* owner);
	bool Use(float fX, float fY, float fAngle) override;
	void HoldUpdate() override;
	void HoldRender() override;
};