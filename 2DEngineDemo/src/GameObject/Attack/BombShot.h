#pragma once
#include "Attack.h"

class Vector2;

class BombShot :
    public Attack
{
public:
    BombShot(GameScene& gs, const Vector2& startPos, const float& angle);
    ~BombShot() = default;
    void Initialize() override;
    void SetDamage(const int& damage) override;
    int GetDamage() const override;
    void Update(const float& deltaTime) override;
};
