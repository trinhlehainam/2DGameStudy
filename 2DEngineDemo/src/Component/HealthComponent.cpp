#include "HealthComponent.h"


HealthComponent::HealthComponent(const std::shared_ptr<Entity>& owner, const int& maxHealth):
	health_(maxHealth),maxHealth_(maxHealth),Component(owner)
{
}

void HealthComponent::Initialize()
{
}

void HealthComponent::Update(const float& deltaTime)
{
}

void HealthComponent::Render()
{
}

void HealthComponent::SetMaxHealth(const int& maxHealth)
{
	maxHealth_ = maxHealth;
}

void HealthComponent::SetHealth(const int& health)
{
	health_ = health;
}

void HealthComponent::TakeDamage(const int& damage)
{
	health_ -= damage;
	receivedDamage_ = damage;
}

void HealthComponent::AddMaxHealth(const int& health)
{
	maxHealth_ += health;
}

void HealthComponent::Heal(const int& health)
{
	health_ += health;
}

