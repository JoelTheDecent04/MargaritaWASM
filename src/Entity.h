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
		None, Player, Bomb, Crab, Enemy, Laser, Orb, Light, Structure, Coin, Turret, Block, Particle, Airstrike, Snail
	};

	enum Status
	{
		KEEPALIVE,
		REMOVE
	};
	
	enum class Category : int
	{
		NONE = 0,
		OTHER = 1,
		PLAYER = 2,
		ENEMY = 4,
		STRUCTURE = 8,
		PROJECTILE = 16,
		DECORATION = 32
	};
	


protected:
	Type nType;
	
public:
	Entity(const Entity&) = delete;
	Entity& operator=(const Entity&) = delete;
	Texture* texture;
	int nFrame;
	float fX, fY;
	float fSpeedX, fSpeedY;
	bool bOnGround;
	bool bAffectedByGravity;
	bool bBouncy;
	bool bCanPenetrate;
	//bool bCanCollide;
	//bool bCanCollideWithPlayer;

	Category nCollisionCategories;
	Category nCategory;

	float fHealth;
	float fMaxHealth;
	float fHeight, fWidth;
	bool bIsBullet;
	float fCustomGravity;	

	Entity(Texture* texture, float fX, float fY, Type nType, Category nCategory);
	virtual Status Update(float deltatime);
	virtual void ChangeHealth(float fChange, Entity* e);
	virtual bool Collide(Entity& entity) { return true; };
	virtual bool ShouldCollideWith(Entity& entity);
	virtual void Draw();
	bool Overlapping(Entity& e);
	bool WillOverlap(Entity& e, float fNewX, float fNewY);
	float Distance(Entity& entity);
	virtual void OnDestroy() {};
	void Destroy() {fHealth = 0.0f; };
	virtual void LandOnGround() {};
	virtual bool IsEnemy() { return false; }
	Type GetType() const { return nType; }
};

extern const char* astrEntityName[8];

static inline Entity::Category operator|(Entity::Category lhs, Entity::Category rhs){
	return (Entity::Category)( (int)lhs | (int)rhs );
}
static inline bool operator&(Entity::Category lhs, Entity::Category rhs){
	return (bool)( (int)lhs & (int)rhs );
}