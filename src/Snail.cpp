#include "Snail.h"
#include "Space.h"
#include "Player.h"
#include "Laser.h"

Snail::Snail()
    : Entity(Textures::Snail, 5200, 594 - 30, Entity::Type::Snail, Category::ENEMY)
{
    nCollisionCategories = Category::PLAYER;
    fHealth = 1000.0f;
    fMaxHealth = 1000.0f;
    bAffectedByGravity = false;
    fSecondsUntilNextAttack = 0.0f;
    pathfinding = true;
}

Entity::Status Snail::Update(float deltatime)
{
    auto& player = SpaceGame::Instance().GetPlayer();
    float dist_to_player = Distance(player);

    if (abs(player.fX - fX) < 10.0f)
        pathfinding = false;

    if (abs(player.fX - fX) > 400.0f)
        pathfinding = true;
    
    if (pathfinding)
		fSpeedX = player.fX > fX ? 300.0f: -300.0f;
	else
	{
		fSpeedX = 0.0f;
		if (fSecondsUntilNextAttack > 0.0f)
			fSecondsUntilNextAttack -= deltatime;
		else
        {
            SpaceGame::Instance().Entities.Add(new LaserBeam(fX, fY, 3000.f * (player.fX - fX) / dist_to_player, 3000.f * (player.fY - fY) / dist_to_player, LaserBeam::LaserType::DamagePlayers));
		    fSecondsUntilNextAttack = 5.0f;
        }
    }

    //Turn based on direction moving
    if (fSpeedX < 0)
        nFrame = 0;
    if (fSpeedX > 0)
        nFrame = 1;

    return Entity::Update(deltatime);
}

bool Snail::ShouldCollideWith(Entity& entity)
{
    return entity.GetType() == Entity::Type::Snail;
}