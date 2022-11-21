#include "Item.h"

Item::Item(Entity* owner)
{ 
	nCount = 0; 
	nType = Type::None;
	strName = "Unknown";
	this->owner = owner;
	texture = nullptr;
}
