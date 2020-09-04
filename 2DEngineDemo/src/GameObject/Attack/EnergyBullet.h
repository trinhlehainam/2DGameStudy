#pragma once
#include "Attack.h"

class EnergyBullet :
    public Attack
{
private:

public:
    EnergyBullet(GameScene& gs, const std::shared_ptr<Entity>& owner, const Vector2& startPos,
        const float& angle, const int& damage = 0);
    ~EnergyBullet() = default;
    void Initialize() override;
    void SetDamage(const int& damage) override;
    int GetDamage() const override;
    void Update(const float& deltaTime) override;
};

