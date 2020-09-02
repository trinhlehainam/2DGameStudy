#pragma once
#include "Equipment.h"

class BombEquip :
    public Equipment
{
public:
    BombEquip(GameScene& gs, const std::string& tag, const std::shared_ptr<Entity>&,const int&);
    ~BombEquip() = default;
    void Initialize() override;
    void Attack(const Vector2& startPos, const float& dirAngle, const float& w, const float& h) override;
    void Render() override;
};

