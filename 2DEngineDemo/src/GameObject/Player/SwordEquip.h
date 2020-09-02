#pragma once
#include "Equipment.h"

class SwordEquip :
    public Equipment
{
public:
    SwordEquip(GameScene&, const std::string&, const std::shared_ptr<Entity>& owner, const int& damage = 0);
    ~SwordEquip() = default;

    void Initialize() override;
    void Attack(const Vector2& startPos, const float& dirAngle, const float& w, const float& h) override;
    void Render() override;
};

