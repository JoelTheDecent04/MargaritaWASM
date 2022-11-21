#include "Coin.h"
#include "Space.h"
#include "Player.h"

CoinReward::CoinReward(float fX, float fY) 
: Entity(Textures::Coin, fX, fY, Type::Coin, Category::DECORATION)
{
    nFrame = 0;
    nCollisionCategories = Category::NONE;
    bAffectedByGravity = false; //Does its own gravity calculation
    bBouncy = false;
    fSpeedY = -275.0f;
}

Entity::Status CoinReward::Update(float delta)
{
    if (Distance(SpaceGame::Instance().GetPlayer()) < 40.0f) {
        SpaceGame::Instance().GetPlayer().fMoney += 50.0f;
        return Status::REMOVE;  
    }

    if (fSpeedY < 0)
		fSpeedY += fGravity * delta * 4;
	else
		fSpeedY += fGravity * delta * 7;

    return Entity::Update(delta);
}

//--------------------------------------------------------------------------------
CoinPickup::CoinPickup(float fX, float fY)
 : Entity(Textures::Coin, fX, fY, Type::Coin, Category::DECORATION)
{
    nFrame = 1;
    nCollisionCategories = Category::NONE;
    bAffectedByGravity = false;
}
Entity::Status CoinPickup::Update(float deltatime)
{
    fTimeInSpin += deltatime;
    fTimeInSpin = fmod(fTimeInSpin, 1.0f);

    if (SpaceGame::Instance().WaveSecondsLeft() == 0.0f)
        return Status::REMOVE;

    if (Distance(SpaceGame::Instance().GetPlayer()) < 40.0f) {
        SpaceGame::Instance().GetPlayer().fMoney += 100.0f;
        OnDestroy();
        return Status::REMOVE;  
    }
    return Status::KEEPALIVE;
}
void CoinPickup::Draw()
{
    float width = texture->fTextureDrawnWidth * sin(fTimeInSpin * 3.14159f);
    if (SpaceGame::Instance().ShowingHitboxes())
		Graphics::FillRectangle(fScaleH * (fX - fWidth / 2 - SpaceGame::RenderPosition()), fScaleV * (fY - fHeight / 2), fScaleH * fWidth, fScaleV * fHeight, clrWhite, 0.5f);
	texture->DrawDifferentSize(
        nFrame,
        fX - SpaceGame::RenderPosition() - width / 2.f,
        fY - texture->fTextureDrawnHeight / 2,
        width, 
        32.0f
    );
}

void CoinPickup::OnDestroy()
{
    SpaceGame::Instance().Entities.Add(new CoinPickup(randomf() * 5120.f, randomf() * 400.f));
}