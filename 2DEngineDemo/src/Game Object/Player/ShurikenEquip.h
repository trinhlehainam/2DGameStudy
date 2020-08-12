#pragma once
#include "Equipment.h"

class ShurikenEquip :
    public Equipment
{
public:
    ShurikenEquip(GameScene&);
    ~ShurikenEquip();

    void Attack(const Vector2& startPos_, const float& angle) override;
    void Initialize() override;
    void Render() override;
};

