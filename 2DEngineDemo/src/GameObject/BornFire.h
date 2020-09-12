#pragma once
#include "CheckPoint.h"
class BornFire :
    public CheckPoint
{
public:
    BornFire(GameScene& gs);
    ~BornFire() = default;
    void Initialize() override;
    void Update(const float&) override;
    void Render() override;
};

