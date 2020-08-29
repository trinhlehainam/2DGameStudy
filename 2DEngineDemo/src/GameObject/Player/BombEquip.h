#pragma once
#include "Equipment.h"

class BombEquip :
    public Equipment
{
private:

public:
    BombEquip(GameScene& gs, const std::string& tag);
    ~BombEquip() = default;
    void Initialize() override;
    void Attack(const Vector2& startPos, const float& angle) override;
    void Render() override;
};

