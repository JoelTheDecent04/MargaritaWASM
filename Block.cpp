#include "Block.h"
#include "Space.h"

Block::Block(float x, float y)
	: Entity(TextureID::Block, x, y)
{
	bAffectedByGravity = false;
	nType = Entity::Type::Structure;
	bCanCollideWithPlayer = true;
	bCanCollide = true;
}

BlockItem::BlockItem(Entity* owner)
	: Item(owner)
{
	nTexture = TextureID::None;
	nCount = 1;
	nType = Type::Block;
	strName = "Block";
	
}

bool BlockItem::Use(float fX, float fY, float fAngle)
{
	SpaceGame::Instance().AddEntity(new Block(world_x, world_y));
	return true;
}

void BlockItem::HoldUpdate()
{
	int x, y;
	GetRelativeMousePos(&x, &y);
	float mouse_world_x, mouse_world_y;
	mouse_world_y = y;
	mouse_world_x = x + SpaceGame::RenderPosition();

	float block_position_x = mouse_world_x / 32.0f;
	float block_position_y = (mouse_world_y) / 32.0f;
	block_position_x = round(block_position_x);
	block_position_y = round(block_position_y);
	
	world_x = SDL_clamp(block_position_x * 32.0f, 0, 5088);
	world_y = SDL_clamp(block_position_y * 32.0f, 0, 578);
}

void BlockItem::HoldRender()
{
	textures[TextureID::Bomb]->Draw(0, world_x - SpaceGame::RenderPosition() - 16.0f, world_y - 16.0f, false, 0);
}


