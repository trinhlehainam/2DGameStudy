#pragma once
#include "../Component.h"

class AttackComponent :
    public Component
{
protected:
    int damage_;
    float impact_;
public:
    AttackComponent(const std::shared_ptr<Entity> owner);
    virtual ~AttackComponent() = default;
    virtual void Initialize() override = 0;
    virtual void Update(const float& deltaTime) override= 0;
    virtual void Render() override = 0;

    inline int GetDamage() const { return damage_; }
    inline void SetDamage(const int& damage) { damage_ = damage; }
    inline float GetImpact() const { return impact_; }
    inline void SetImpact(const float& impact) { impact_ = impact; }
};

