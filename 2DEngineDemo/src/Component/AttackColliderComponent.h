#pragma once
#include "ColliderComponent.h"
class AttackColliderComponent :
    public ColliderComponent
{
public:
    Rect collider_;
    Rect destRect_;
    int damage_;
    int lifeTime_;

    void Initialize() override;
    void Update(const float& deltaTime) override;
    void Render() override;

    AttackColliderComponent(std::shared_ptr<Entity> owner, const Vector2& pos, const float& w, const float& h);
    ~AttackColliderComponent() = default;
};

