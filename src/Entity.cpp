#include "Entity.h"
#include "Space.h"
#include "EntityHealthChangeText.h"
#include "Utilities.h"
#include "Colours.h"


const char* astrEntityName[8] = { "None", "Player", "Bomb", "Crab", "Alien", "Laser", "Orb", "Light" };

Entity::Entity(Texture* texture, float fX, float fY, Type nType, Category nCategory)
{
	this->texture = texture;
	this->fX = fX;
	this->fY = fY;
	this->nType = nType;
	this->nCategory = nCategory;
	nFrame = 0;
	fSpeedX = 0.0f;
	fSpeedY = 0.0f;
	bOnGround = false;
	bAffectedByGravity = true;
	fHealth = -1.0f;
	fMaxHealth = 100.0f;
	fHeight = texture->fTextureDrawnHeight;
	fWidth = texture->fTextureDrawnWidth;
	nCollisionCategories = nCategory;
	bBouncy = true;
	bIsBullet = false;
	fCustomGravity = fGravity;
	bCanPenetrate = false;
}
void Entity::Draw()
{
	if (SpaceGame::Instance().ShowingHitboxes())
		Graphics::FillRectangle(fScaleH * (fX - fWidth / 2 - SpaceGame::RenderPosition()), fScaleV * (fY - fHeight / 2), fScaleH * fWidth, fScaleV * fHeight, clrWhite, 0.5f);
	texture->Draw(nFrame, (fX - SpaceGame::RenderPosition() - (texture->fTextureDrawnWidth / 2)), fY - texture->fTextureDrawnHeight / 2);
}

Entity::Status Entity::Update(float deltatime)
{
	if (fHealth == 0.0f)
	{
		OnDestroy();
		return Status::REMOVE;
	}

	if (bAffectedByGravity) fSpeedY += fCustomGravity * deltatime; //Gravity force

	float fNewX = fX + fSpeedX * deltatime; //Get new position
	float fNewY = fY + fSpeedY * deltatime;

	if (nCollisionCategories != Category::NONE) //If this entity can collide
	{
 		bool bCollidedVertically = false;
		for (auto& entity : SpaceGame::Instance().Entities) //Check if it can move vertically
		{
			if (&entity == this) continue;
			if (!ShouldCollideWith(entity)) continue;
			if (entity.WillOverlap(*this, fX, fNewY))
			{
				if (Collide(entity) == false)
					return Status::REMOVE;
				
				bCollidedVertically = true;
				break;
			}
		}
		if (bCollidedVertically && !bCanPenetrate)
			;
			//fSpeedY = 0.0f;
		else
			fY = fNewY;


		bool bCollidedHorizontally = false;
		for (auto& entity : SpaceGame::Instance().Entities) //Check if it can move horizontally
		{
			if (&entity == this) continue;
			if (!ShouldCollideWith(entity)) continue;
			if (entity.WillOverlap(*this, fNewX, fY))
			{
				if (Collide(entity) == false)
					return Status::REMOVE;

				bCollidedHorizontally = true;
				break;
			}
		}
		if (bCollidedHorizontally && !bCanPenetrate) 
			;
			//fSpeedX = 0.0f;
		else
			fX = fNewX;
	}
	else
	{
		fX = fNewX;
		fY = fNewY;
	}

	if (fY >= 594.0f - fHeight / 2) //If below ground
	{
		if (bBouncy)
		{
			if (fSpeedY - fCustomGravity * deltatime < 5.0f) //Set speed to 0 if it is already very low
				fSpeedY = 0.0f;

			fY = 594.0f - fHeight / 2;
			fSpeedY = -(fSpeedY) * 0.6f; //bounce

			if (fSpeedX > 0.0f)
			{
				fSpeedX -= fFriction * deltatime;
				if (fSpeedX < 0.0f) fSpeedX = 0.0f;
			}
			if (fSpeedX < 0.0f)
			{
				fSpeedX += fFriction * deltatime;
				if (fSpeedX > 0.0f) fSpeedX = 0.0f;
			}
		}
		else
		{
			fY = 594.0f - fHeight / 2;
			LandOnGround();
		}
	}

	if (fX < -80.0f || fX > 5200.0f || fY < -250.0f)
		return Status::REMOVE; 

	if (fSpeedX < 5.0f && fSpeedX > -5.0f) fSpeedX = 0.0f; //If speed is low set to 0

	return Status::KEEPALIVE;
}

void Entity::ChangeHealth(float fChange, Entity* e)
{
	SpaceGame::Instance().BackgroundObjects.Add(new EntityHealthChangeText(this, fChange));
	fHealth += fChange;
	if (fHealth <= 0.0f)
	{
		SpaceGame::Instance().EventHandler().Event(EventType::Kill, 0, e, this);
		fHealth = 0.0f; //When OnDestroy() is called on the player, it won't get deleted straight away
	}
	else if (fChange < 0.0f)
	{
		SpaceGame::Instance().EventHandler().Event(EventType::Hit, 0, e, this);

	}
}

bool Entity::ShouldCollideWith(Entity& entity)
{
	return (nCollisionCategories & entity.nCategory); 
}

bool Entity::Overlapping(Entity& e)
{
	return (fX - fWidth / 2 < e.fX + e.fWidth / 2 &&
		fX + fWidth / 2 > e.fX - e.fWidth / 2 &&
		fY - fHeight / 2 < e.fY + e.fHeight / 2 &&
		fY + fHeight / 2> e.fY - e.fHeight / 2);
}

bool Entity::WillOverlap(Entity& e, float fNewX, float fNewY)
{
	return (fX - fWidth / 2 < fNewX + e.fWidth / 2 &&
		fX + fWidth / 2 > fNewX - e.fWidth / 2 &&
		fY - fHeight / 2 < fNewY + e.fHeight / 2 &&
		fY + fHeight / 2 > fNewY - e.fHeight / 2);
}

float Entity::Distance(Entity& entity)
{
	return distance(fX, fY, entity.fX, entity.fY);
}
