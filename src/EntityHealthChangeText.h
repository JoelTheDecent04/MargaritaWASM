#pragma once
#include "TextObject.h"
#include "Entity.h"

class EntityHealthChangeText : public TextObject
{
public:
	EntityHealthChangeText(Entity* entity, int nChange);
};