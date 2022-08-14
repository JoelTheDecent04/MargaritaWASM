#include "Coin.h"
#include "Space.h"
#include "Player.h"

CoinReward::CoinReward(float fX, float fY) 
: Entity(TextureID::Coin, fX, fY)
{
    nFrame = 0;
    bCanCollide = false;
    bCanCollideWithPlayer = false;
    bAffectedByGravity = false; //Does its own gravity calculation
    bBouncy = false;
    fSpeedY = -275.0f;
}

bool CoinReward::Update(float delta)
{
    if (Distance(SpaceGame::Instance().GetPlayer()) < 40.0f) {
        SpaceGame::Instance().GetPlayer()->fMoney += 50.0f;
        return false;  
    }

    if (fSpeedY < 0)
		fSpeedY += fGravity * delta * 4;
	else
		fSpeedY += fGravity * delta * 7;

    return Entity::Update(delta);
}

//--------------------------------------------------------------------------------
CoinPickup::CoinPickup(float fX, float fY)
 : Entity(TextureID::Coin, fX, fY)
{
    nFrame = 1;
    bCanCollide = false;
    bCanCollideWithPlayer = false;
    bAffectedByGravity = false;
}
bool CoinPickup::Update(float deltatime)
{
    fTimeInSpin += deltatime;
    fTimeInSpin = fmod(fTimeInSpin, 1.0f);

    if (SpaceGame::Instance().WaveSecondsLeft() == 0.0f)
        return false;

    if (Distance(SpaceGame::Instance().GetPlayer()) < 40.0f) {
        SpaceGame::Instance().GetPlayer()->fMoney += 100.0f;
        OnDestroy();
        return false;  
    }
    return true;
}
void CoinPickup::Draw()
{
    float width = textures[nTexture]->fTextureDrawnWidth * sin(fTimeInSpin * 3.14159f);
    if (SpaceGame::Instance().ShowingHitboxes())
		Graphics::FillRectangle(fScaleH * (fX - fWidth / 2 - SpaceGame::RenderPosition()), fScaleV * (fY - fHeight / 2), fScaleH * fWidth, fScaleV * fHeight, clrWhite, 0.5f);
	textures[nTexture]->DrawDifferentSize(
        nFrame,
        fX - SpaceGame::RenderPosition() - width / 2.f,
        fY - textures[nTexture]->fTextureDrawnHeight / 2,
        width, 
        32.0f
    );
    //textures[nTexture]->Draw(nFrame, (fX - SpaceGame::RenderPosition() - (textures[nTexture]->fTextureDrawnWidth / 2)), fY - textures[nTexture]->fTextureDrawnHeight / 2);

}

void CoinPickup::OnDestroy()
{
    SpaceGame::Instance().AddEntity(new CoinPickup(randomf() * 5120.f, randomf() * 400.f));
}