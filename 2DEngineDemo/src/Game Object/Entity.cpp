#include "Entity.h"

#include "../Component/ProjectileEmitterComponent.h"


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

Vector2 Entity::GetProjectileVelocity()
{
	if (HasComponent<ProjectileEmitterComponent>())
		return GetComponent<ProjectileEmitterComponent>()->velocity_;
}
