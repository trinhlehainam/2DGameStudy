#pragma once
#include "ColliderComponent.h"
class AttackColliderComponent :
    public ColliderComponent
{
private:
    int damage_;
    int lifeTime_;
public:
    Rect collider_;
    Rect destRect_;
    
    inline void SetDamage(const int& damage) { damage_ = damage; }
    inline void SetLifeTime(const int& time) { lifeTime_ = time; }
    void Initialize() override;
    void Update(const float& deltaTime) override;
    void Render() override;

    AttackColliderComponent(const std::shared_ptr<Entity>& owner, const Vector2& pos, const float& w, const float& h);
    ~AttackColliderComponent() = default;
};

