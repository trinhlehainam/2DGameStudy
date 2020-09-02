#pragma once
#include "Attack.h"

class Vector2;

class ShurikenShot :
    public Attack
{
private:

public:
    ShurikenShot(GameScene& gs, const std::shared_ptr<Entity>& owner, const Vector2& startPos, 
        const float& angle, const int& damage = 0);
    ~ShurikenShot() = default;
    void Initialize() override;
    void SetDamage(const int& damage) override;
    int GetDamage() const override;
    void Update(const float& deltaTime) override;
};

