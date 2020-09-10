#pragma once
#include "Enemy.h"

class RigidBody2D;

class Skeleton :
    public Enemy
{
private:
    using ActionUpdate_t = void (Skeleton::*)(const float& deltaTime);
    ActionUpdate_t actionUpdate_;
    std::shared_ptr<RigidBody2D> rigidBody_;

    float timer_ = 0;
    float startPosX_ = 0;

    void HurtUpdate(const float&);
    void DeathUpdate(const float&);
    void PatrollingUpdate(const float&);
    void AimPlayerUpdate(const float&);
    void AttackUpdate(const float&);
    void WaitForDestroyUpdate(const float&);

    void CheckHit();
public:
    Skeleton(GameScene& gs, const std::shared_ptr<TransformComponent>& playerPos);
    ~Skeleton();

    void Initialize() override;
    void Update(const float& deltaTime) override;
    std::unique_ptr<Enemy> MakeClone() override;
    void SetPosition(const Vector2& pos) override;
};

