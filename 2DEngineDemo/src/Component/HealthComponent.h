#pragma once
#include "Component.h"

/// <param name="maxHealth"></param>
class HealthComponent :
    public Component
{
private:
    int health_;
    int maxHealth_;
public:

    HealthComponent(const std::shared_ptr<Entity>& owner, const int& maxHealth);
    ~HealthComponent() = default;

    void Initialize() override;
    void Update(const float& deltaTime) override;
    void Render() override;

    void SetMaxHealth(int maxHealth);
    inline int GetMaxHealth() const { return maxHealth_; }
    void SetHealth(int health);
    inline int GetHealth() const { return health_; }

    void TakeDamage(int damage);
    void AddMaxHealth(int health);
    void Heal(int health);
};

