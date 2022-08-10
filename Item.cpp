#include "Item.h"

Item::Item(const std::shared_ptr<Entity>& owner)
{ 
	nCount = 0; 
	nType = Type::None;
	strName = "Unknown";
	this->owner = owner;
	nTexture = TextureID::None;
}
