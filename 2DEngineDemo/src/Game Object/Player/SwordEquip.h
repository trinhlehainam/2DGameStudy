#pragma once
#include "Equipment.h"

class SwordEquip :
    public Equipment
{
public:
    SwordEquip(GameScene&, const std::string&);
    ~SwordEquip() = default;

    void Initialize() override;
    void Attack(const Vector2& startPos, const float& angle) override;
    void Render() override;
};

