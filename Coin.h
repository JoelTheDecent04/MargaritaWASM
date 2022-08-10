#pragma once
#include "Entity.h"
#include "Particle.h"

class CoinReward : public Entity
{
public:
    CoinReward(float fX, float fY);
    bool Update(float deltatime) override;
};

class CoinPickup : public Entity
{
    float fTimeInSpin = 0.0f;
public:
    CoinPickup(float fX, float fY);
    bool Update(float deltatime) override;
    void Draw() override;
    void OnDestroy() override;
};