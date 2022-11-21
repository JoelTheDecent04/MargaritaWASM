#include "Laser.h"
#include "Space.h"
#include "Player.h"
#include <math.h>
#define PI 3.1415926f

LaserBeam::LaserBeam(float fX, float fY, float fSpeedX, float fSpeedY, LaserType nLaserType) //TODO change to target specific entity
	: Entity(nLaserType == LaserType::DamageEnemies ? Textures::LaserBeam : Textures::EnemyLaser, fX, fY, Type::Laser, Category::PROJECTILE)
{
	bAffectedByGravity = false;
	this->fSpeedX = fSpeedX;
	this->fSpeedY = fSpeedY;
	nCollisionCategories = Category::ENEMY | Category::STRUCTURE | Category::PLAYER;
	bIsBullet = true;

	fWidth = 0.0f;
	fHeight = 0.0f;

	fAngle = atan2(fSpeedY, fSpeedX) * 180 / 3.14159f;

	nType = Type::Laser;
	this->nLaserType = nLaserType;
}

bool LaserBeam::Collide(Entity& entity)
{
	if (((entity.GetType() == Entity::Type::Player || entity.GetType() == Entity::Type::Structure) && nLaserType == LaserType::DamagePlayers) ||
		(entity.GetType() != Entity::Type::Player && nLaserType == LaserType::DamageEnemies))
	{
		entity.ChangeHealth(-50.0f, this);
		OnDestroy();
		return false;
	}

	
	return true;
}

void LaserBeam::Draw()
{
	texture->Draw(nFrame, (fX - SpaceGame::RenderPosition() - (texture->fTextureDrawnWidth / 2)), fY - texture->fTextureDrawnHeight / 2, false, fAngle);
}

Entity::Status LaserBeam::Update(float deltatime)
{
	float fBulletX = fX + cos(PI * fAngle / 180.0f) * 16.0f; //Get new position
	float fBulletY = fY + sin(PI * fAngle / 180.0f) * 16.0f;

	float fNewX = fX + fSpeedX * deltatime;
	float fNewY = fY + fSpeedY * deltatime;

	bool bCollided = false;
	for (auto& entity : SpaceGame::Instance().Entities) //Check for collisions
	{
		if (&entity == this) continue;
		if (entity.nCategory & this->nCollisionCategories)
		{
			if (entity.WillOverlap(*this, fBulletX, fBulletY))
			{
				bCollided = true;
				if (Collide(entity) == false)
					return Status::REMOVE;
			}
		}
	}

	fX = fNewX;
	fY = fNewY;

	bool bSpeedChanged = false;

	if (fBulletY >= 594.0f) //If below ground
	{
		fY = 594.0f - sin(PI * fAngle / 180.0f) * 16.0f;
		fX += 2.0f * cos(PI * fAngle / 180.0f) * 16.0f;
		fSpeedY = -(fSpeedY) * 0.6f; //bounce
		bSpeedChanged = true;
	}

	if (fX < 0.0f || fX > 5120.0f) { OnDestroy(); return Status::REMOVE; }

	if (bSpeedChanged)
	{
		float fGradient = fSpeedY / fSpeedX;
		fAngle = atan(fGradient) * 180 / 3.1415926f;
		if (fSpeedX < 0.0f)
			fAngle += 180.0f;
	}

	if (fX < 0.0f || fX > 5120.0f || fY < -250.0f)
		return Status::REMOVE;
	
	return Status::KEEPALIVE;
}

LaserWeapon::LaserWeapon(Entity* owner, LaserLevel nLaserLevel)
	: Weapon(owner)
{
	texture = Textures::Laser;
	this->nLaserLevel = nLaserLevel;
	nCount = -1;
	nType = Type::Laser;
	strName = "Laser";
}

bool LaserWeapon::Use(float fX, float fY, float fAngle)
{
	auto* weapon_owner = GetOwner();
	Player* player = weapon_owner->GetType() == Entity::Type::Player ? (Player*)weapon_owner : nullptr;

	switch (nLaserLevel)
	{
	case LaserLevel::Normal:
		
		if (!player || player->fEnergy >= 4)
		{
			SpaceGame::Instance().Entities.Add(new LaserBeam(fX, fY, 1000.0f * cos(fAngle), 1000.0f * sin(fAngle)));

			if (player)
			player->fEnergy -= 4;
		}
		break;
	case LaserLevel::DoubleShot:
		if (!player || player->fEnergy >= 6)
		{
			//auto* callback = new Callback(std::static_pointer_cast<LaserWeapon>(shared_from_this()), &LaserWeapon::Fire,  owner, fAngle );
			//SpaceGame::Instance().EventHandler().RegisterCallback(callback, 100);
			SpaceGame::Instance().EventHandler().DelayedFunctionInvoke([] (void* ptr) {
				float* angle = (float*)ptr;
				auto& player = SpaceGame::Instance().GetPlayer();
				SpaceGame::Instance().Entities.Add(new LaserBeam(player.fX, player.fY, 1000.0f * cos(*angle), 1000.0f * sin(*angle)));
				delete angle;
			}, 100.f, new float(fAngle));
			
			SpaceGame::Instance().Entities.Add(new LaserBeam(fX, fY, 1000.0f * cos(fAngle), 1000.0f * sin(fAngle)));
			if (player)
				player->fEnergy -= 6;
		}
		break;
	}
	return true;
}

List<ItemUpgrade> LaserWeapon::GetPossibleUpgrades()
{
	List<ItemUpgrade> upgrades;

	if (nLaserLevel == LaserLevel::Normal)
		upgrades.Add(new ItemUpgrade(400, *(int*)&nLaserLevel, "Laser Double Shot", Textures::Laser, 1));

	return upgrades;
}

void LaserWeapon::HoldUpdate()
{
	nTextureFrame = (int)nLaserLevel - 1;
}

void LaserWeapon::Fire(std::weak_ptr<Entity> owner, float fAngle)
{
	auto ptr = owner.lock();
	if (ptr)
		SpaceGame::Instance().Entities.Add(new LaserBeam(ptr->fX, ptr->fY, 1000.0f * cos(fAngle), 1000.0f * sin(fAngle)));
}

