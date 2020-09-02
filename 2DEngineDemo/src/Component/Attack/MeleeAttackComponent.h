#pragma once
#include "AttackComponent.h"

class MeleeAttackComponent :
    public AttackComponent
{
private:
    int lifeTime_;
public:
    inline void SetLifeTime(const int& time) { lifeTime_ = time; }

    void Initialize() override;
    void Update(const float& deltaTime) override;
    void Render() override;

    MeleeAttackComponent(const std::shared_ptr<Entity>& owner, const int& damage);
    ~MeleeAttackComponent() = default;
};

