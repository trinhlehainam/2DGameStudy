#pragma once
#include "Component.h"

#include "../Geometry/Geometry.h"

class ProjectileEmitterComponent :
    public Component
{
private:
    friend class Entity;

    Vector2 start_;
    Vector2 velocity_;
    float range_;
    int damage_;

    bool IsOutOfWorld();
public:
    ProjectileEmitterComponent(Entity& owner,const Vector2& startPos, const Vector2& velocity,
        const float& range, const int& damage);
    ~ProjectileEmitterComponent() = default;
    inline int GetDamage() const { return damage_; }
    void Initialize() override;
    void Update(const float& deltaTime) override;
    void Render() override;
};

