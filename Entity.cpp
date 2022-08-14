#include "Entity.h"
#include "Space.h"
#include "EntityHealthChangeText.h"
#include "Utilities.h"
#include "Colours.h"


const char* astrEntityName[8] = { "None", "Player", "Bomb", "Crab", "Alien", "Laser", "Orb", "Light" };

Entity::Entity(int nTexture, float fX, float fY)
{
	this->nTexture = nTexture;
	this->fX = fX;
	this->fY = fY;
	nFrame = 0;
	fSpeedX = 0.0f;
	fSpeedY = 0.0f;
	bOnGround = false;
	bAffectedByGravity = true;
	fHealth = -1.0f;
	fMaxHealth = 100.0f;
	fHeight = textures[nTexture]->fTextureDrawnHeight;
	fWidth = textures[nTexture]->fTextureDrawnWidth;
	bCanCollide = true;
	bCanCollideWithPlayer = false;
	bBouncy = true;
	bIsBullet = false;
	fCustomGravity = fGravity;

	nType = Type::None;
}
void Entity::Draw()
{
	if (SpaceGame::Instance().ShowingHitboxes())
		Graphics::FillRectangle(fScaleH * (fX - fWidth / 2 - SpaceGame::RenderPosition()), fScaleV * (fY - fHeight / 2), fScaleH * fWidth, fScaleV * fHeight, clrWhite, 0.5f);
	textures[nTexture]->Draw(nFrame, (fX - SpaceGame::RenderPosition() - (textures[nTexture]->fTextureDrawnWidth / 2)), fY - textures[nTexture]->fTextureDrawnHeight / 2);
}

bool Entity::Update(float deltatime)
{
	if (fHealth == 0.0f)
	{
		OnDestroy();
		return false;
	}

	if (bAffectedByGravity) fSpeedY += fCustomGravity * deltatime; //Gravity force

	float fNewX = fX + fSpeedX * deltatime; //Get new position
	float fNewY = fY + fSpeedY * deltatime;

	if (bCanCollide || bCanCollideWithPlayer) //If this entity can collide
	{
 		bool bCollidedVertically = false;
		for (auto* entity : SpaceGame::Instance().Entities()) //Check if it can move vertically
		{
			if (entity == this || !entity->bCanCollide) continue;
			if ((entity->GetType() == Type::Player && !bCanCollideWithPlayer) || (entity->GetType() != Type::Player && !bCanCollide)) continue;
			if (entity->WillOverlap(this, fX, fNewY) && this->ShouldHit(entity))
			{
				if (Collide(entity) == false)
					return false;
				
				bCollidedVertically = true;
				break;
			}
		}
		if (bCollidedVertically)
			;
			//fSpeedY = 0.0f;
		else
			fY = fNewY;


		bool bCollidedHorizontally = false;
		for (auto* entity : SpaceGame::Instance().Entities()) //Check if it can move horizontally
		{
			if (entity == this || !entity->bCanCollide) continue;
			if ((entity->GetType() == Type::Player && !bCanCollideWithPlayer) || (entity->GetType() != Type::Player && !bCanCollide)) continue;
			if (entity->WillOverlap(this, fNewX, fY) && this->ShouldHit(entity))
			{
				if (Collide(entity) == false)
					return false;

				bCollidedHorizontally = true;
				break;
			}
		}
		if (bCollidedHorizontally) 
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

	if (fX < 0.0f || fX > 5120.0f || fY < -250.0f)
	{ 
		return false; 
	}

	if (fSpeedX < 5.0f && fSpeedX > -5.0f) fSpeedX = 0.0f; //If speed is low set to 0

	return true;
}

void Entity::ChangeHealth(float fChange, Entity* e)
{
	SpaceGame::Instance().AddBackgroundObject(std::make_shared<EntityHealthChangeText>(this, fChange));
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

bool Entity::Overlapping(Entity* e)
{
	return (fX - fWidth / 2 < e->fX + e->fWidth / 2 &&
		fX + fWidth / 2 > e->fX - e->fWidth / 2 &&
		fY - fHeight / 2 < e->fY + e->fHeight / 2 &&
		fY + fHeight / 2> e->fY - e->fHeight / 2);
}

bool Entity::WillOverlap(Entity* e, float fNewX, float fNewY)
{
	return (fX - fWidth / 2 < fNewX + e->fWidth / 2 &&
		fX + fWidth / 2 > fNewX - e->fWidth / 2 &&
		fY - fHeight / 2 < fNewY + e->fHeight / 2 &&
		fY + fHeight / 2 > fNewY - e->fHeight / 2);
}

float Entity::Distance(Entity* entity)
{
	return distance(fX, fY, entity->fX, entity->fY);
}
