#pragma once
#include "Attack.h"

class Vector2;

class ShurikenShot :
    public Attack
{
private:

public:
    ShurikenShot(GameScene& gs, const Vector2& startPos, const float& angle);
    ~ShurikenShot() = default;
    void Initialize() override;
    void SetDamage(const int& damage) override;
    int GetDamage() const override;
    void Update(const float& deltaTime) override;
};

