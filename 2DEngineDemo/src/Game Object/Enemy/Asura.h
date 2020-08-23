#pragma once
#include "Enemy.h"
#include <vector>

class CircleColliderComponent;

class Asura :
    public Enemy
{
private:
    std::vector<CircleColliderComponent>* colliders_ = nullptr;

    using Updater_t = void (Asura::*)(const float&);
    Updater_t updater_;

    void EnteringUpdate(const float& deltaTime);
    void NormalUpdate(const float& deltaTime);
    void DamageUpdate(const float& deltaTime);
    void ExitingUpdate(const float& deltaTime);
    void DeadUpdate(const float& deltaTime);
public:
    Asura(GameScene& gs, TransformComponent& playerPos_);
    ~Asura();

    void Initialize() override;
    void Update(const float& deltaTime) override;
    std::unique_ptr<Enemy> MakeClone() override;
    void SetPosition(const Vector2& pos) override;
};

