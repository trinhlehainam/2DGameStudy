#pragma once
#include "Equipment.h"

class ShurikenEquip :
    public Equipment
{
public:
    ShurikenEquip(GameScene&,const std::string&, const std::shared_ptr<Entity>&, const int&);
    ~ShurikenEquip();

    void Attack(const Vector2& startPos, const float& dirAngle, const float& w, const float& h) override;
    void Initialize() override;
    void Render() override;
};

