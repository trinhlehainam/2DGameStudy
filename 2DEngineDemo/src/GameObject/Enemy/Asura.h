#pragma once
#include "Enemy.h"
#include <vector>
#include <memory>
#include "../../Geometry/Geometry.h"

class CircleColliderComponent;

struct AsuraAttack
{
    Vector2 pos;
    float angle;
    float waitTimer;
    bool flag = true;
    AsuraAttack(const Vector2& pos, const float& angle, const float& time):pos(pos), angle(angle), waitTimer(time) {}
    ~AsuraAttack() = default;
};

class Asura :
    public Enemy
{
private:
    std::vector<std::shared_ptr<CircleColliderComponent>> colliders_;
    std::vector<Vector2> energyBallPos_;
    std::vector<AsuraAttack> attacks_;

    using Updater_t = void (Asura::*)(const float&);
    Updater_t updater_;
    float cooldown_ = 0;

    void EnteringUpdate(const float& deltaTime);
    void NormalUpdate(const float& deltaTime);
    void DamageUpdate(const float& deltaTime);
    void ExitingUpdate(const float& deltaTime);
    void DeadUpdate(const float& deltaTime);

    void CheckHit();
public:
    Asura(GameScene& gs, const std::shared_ptr<TransformComponent>& playerPos_);
    ~Asura();

    void Initialize() override;
    void Update(const float& deltaTime) override;
    std::unique_ptr<Enemy> MakeClone() override;
    void SetPosition(const Vector2& pos) override;
};

