#pragma once
#include "Texture.h"
#include <fstream>
#include <memory>

class SpaceGame;

class Entity
{
public:
	enum class Type
	{
		None, Player, Bomb, Crab, Enemy, Laser, Orb, Light, Structure
	};

	enum Status
	{
		KEEPALIVE,
		REMOVE
	};
	
protected:
	Type nType;
public:
	int nTexture;
	int nFrame;
	float fX, fY;
	float fSpeedX, fSpeedY;
	bool bOnGround;
	bool bAffectedByGravity;
	bool bBouncy;
	bool bCanCollide;
	bool bCanCollideWithPlayer;
	float fHealth;
	float fMaxHealth;
	float fHeight, fWidth;
	bool bIsBullet;
	float fCustomGravity;

	

	Entity(int nTexture, float fX, float fY);
	virtual Status Update(float deltatime);
	virtual void ChangeHealth(float fChange, Entity* e);
	virtual bool Collide(Entity* entity) { return true; };
	virtual bool ShouldHit(Entity* entity) { return true; };
	virtual void Draw();
	bool Overlapping(Entity* e);
	bool WillOverlap(Entity* e, float fNewX, float fNewY);
	float Distance(Entity* entity);
	virtual void OnDestroy() {};
	void Destroy() {fHealth = 0.0f; };
	virtual void LandOnGround() {};
	virtual bool IsEnemy() { return false; }
	Type GetType() const { return nType; }
};

extern const char* astrEntityName[8];
