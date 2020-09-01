#include "Entity.h"

#include "../Component/Attack/ProjectileEmitterComponent.h"
#include "../Component/HealthComponent.h"


Entity::Entity(EntityManager& entityMng, std::string name) : name_(name), entityMng_(&entityMng)
{
	isActive_ = true;
}

void Entity::Update(const float& deltaTime)
{
	for (auto& component : components_)
	{
		component->Update(deltaTime);
	}
}

void Entity::Render()
{
	for (auto& component : components_)
	{
		component->Render();
	}
}

int Entity::GetProjectileDamage() const
{
	if (HasComponent<ProjectileEmitterComponent>())
	{
		auto& damage = GetComponent<ProjectileEmitterComponent>()->damage_;
		return damage;
	}
	return -1;
}

void Entity::TakeDamage(const int& damage)
{
	auto health = GetComponent<HealthComponent>();
	health->TakeDamage(damage);
	isHit_ = true;
}

Vector2 Entity::GetProjectileVelocity()
{
	if (HasComponent<ProjectileEmitterComponent>())
		return GetComponent<ProjectileEmitterComponent>()->velocity_;
}

void Entity::SetOwnerForAllComponent(const std::shared_ptr<Entity>& owner)
{
	for (auto& component : components_)
	{
		component->SetOwner(owner);
	}
}
