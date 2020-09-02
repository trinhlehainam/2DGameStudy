#pragma once
#include "Attack.h"

class Vector2;

class MeleeAttack :
    public Attack
{
public:
    MeleeAttack(GameScene& gs, const std::shared_ptr<Entity>&, const Vector2& startPos, const float& w,
        const float& h, const int& damage = 0);
     ~MeleeAttack() = default;
    void Initialize() override;
    void SetDamage(const int& damage) override;
    int GetDamage() const override;
    void Update(const float& deltaTime) override;
};

