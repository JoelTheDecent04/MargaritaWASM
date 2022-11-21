#include "Block.h"
#include "Space.h"

Block::Block(float x, float y)
	: Entity(Textures::Block, x, y, Type::Block, Category::STRUCTURE)
{
	bAffectedByGravity = false;
	nType = Entity::Type::Structure;
	//nCollisionCategories = Category::ENEMY | Category::PLAYER | Category::PROJECTILE;
}

void Block::OnDestroy()
{
	SpaceGame::Instance().Blocks[(int)(fX / 32.0f)][(int)(fY / 32.0f)] = 0;
}

BlockItem::BlockItem(Entity* owner)
	: Item(owner)
{
	texture = nullptr;
	nCount = 1;
	nType = Type::Block;
	strName = "Block";
}

bool BlockItem::Use(float fX, float fY, float fAngle)
{
	assert((int)(world_x / 32.0f) >= 0 && (int)(world_x / 32.0f) < 160);
	assert((int)(world_y / 32.0f) >= 0 && (int)(world_y / 32.0f) < 19);
	if (SpaceGame::Instance().Blocks[(int)(world_x / 32.0f)][(int)(world_y / 32.0f)] == 0)
	{
		SpaceGame::Instance().Entities.Add(new Block(world_x, world_y));
		SpaceGame::Instance().Blocks[(int)(world_x / 32.0f)][(int)(world_y / 32.0f)] = 1;
		nCount--;
	}
	return nCount > 0;
}

void BlockItem::HoldUpdate()
{
	float mouse_world_y = Mouse::Y;
	float mouse_world_x = Mouse::X + SpaceGame::RenderPosition();

	float block_position_x = (mouse_world_x) / 32.0f;
	float block_position_y = (mouse_world_y) / 32.0f;
	block_position_x = round(block_position_x);
	block_position_y = round(block_position_y);
	
	world_x = block_position_x * 32.0f;
	world_y = block_position_y * 32.0f;
	//world_x = SDL_clamp(block_position_x * 32.0f, 0, 5088);
	//world_y = SDL_clamp(block_position_y * 32.0f, 0, 578);
}

void BlockItem::HoldRender()
{
	Textures::Block->Draw(0, world_x - SpaceGame::RenderPosition() - 16.0f, world_y - 16.0f, false, 0, 100);
}


