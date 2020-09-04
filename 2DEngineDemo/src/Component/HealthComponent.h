#pragma once
#include "Component.h"

/// <param name="maxHealth"></param>
class HealthComponent :
    public Component
{
private:
    int health_;
    int maxHealth_;
    float takenDamage_;                 // damage resist (unit : percentage)
    int receivedDamage_;                // Latest Damage Received
public:

    HealthComponent(const std::shared_ptr<Entity>& owner, const int& maxHealth);
    ~HealthComponent() = default;

    void Initialize() override;
    void Update(const float& deltaTime) override;
    void Render() override;

    void SetMaxHealth(const int& maxHealth);
    inline int GetMaxHealth() const { return maxHealth_; }
    void SetHealth(const int& health);
    inline int GetHealth() const { return health_; }
    inline int ReceivedDamage() const { return receivedDamage_; }
    void TakeDamage(const int& damage);
    void AddMaxHealth(const int& health);
    void Heal(const int& health);
};

