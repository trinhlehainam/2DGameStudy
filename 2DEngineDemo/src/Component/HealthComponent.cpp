#include "HealthComponent.h"


HealthComponent::HealthComponent(Entity& owner, const int& maxHealth):
	Component(owner),health_(maxHealth),maxHealth_(maxHealth)
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

void HealthComponent::SetMaxHealth(int maxHealth)
{
	maxHealth_ = maxHealth;
}

void HealthComponent::SetHealth(int health)
{
	health_ = health;
}

void HealthComponent::TakeDamage(int damage)
{
	health_ -= damage;
}

void HealthComponent::AddMaxHealth(int health)
{
	maxHealth_ += health;
}

void HealthComponent::Heal(int health)
{
	health_ += health;
}

