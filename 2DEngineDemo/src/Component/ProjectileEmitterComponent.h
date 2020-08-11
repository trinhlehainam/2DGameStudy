#pragma once
#include "Component.h"

#include "../Geometry/Geometry.h"

class ProjectileEmitterComponent :
    public Component
{
private:
    Vector2 start_;
    Vector2 velocity_;
    float range_;

    bool IsOutOfWorld();
public:
    ProjectileEmitterComponent(Entity& owner, Vector2 startPos, Vector2 velocity,const float& range);
    ~ProjectileEmitterComponent() = default;
    void Initialize() override;
    void Update(const float& deltaTime) override;
    void Render() override;
};

