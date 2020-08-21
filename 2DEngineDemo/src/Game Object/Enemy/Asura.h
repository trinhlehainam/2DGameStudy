#pragma once
#include "Enemy.h"
class Asura :
    public Enemy
{
private:

public:
    Asura(GameScene& gs, TransformComponent& playerPos_);
    ~Asura();

    void Initialize() override;
    void Update(const float& deltaTime) override;
    std::unique_ptr<Enemy> MakeClone() override;
    void SetPosition(const Vector2& pos) override;
};

