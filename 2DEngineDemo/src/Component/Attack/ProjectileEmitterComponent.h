#pragma once
#include "AttackComponent.h"

#include "../../Geometry/Geometry.h"

class ProjectileEmitterComponent :
    public AttackComponent
{
private:
    friend class Entity;

    Vector2 start_;
    Vector2 velocity_;
    float range_;

    bool IsOutOfWorld();
public:
    ProjectileEmitterComponent(const std::shared_ptr<Entity>& owner, const Vector2& startPos, 
        const Vector2& velocity, const float& range, const int& damage);
    ~ProjectileEmitterComponent() = default;

    void Initialize() override;
    void Update(const float& deltaTime) override;
    void Render() override;
};

