#include "Entity.h"



Entity::Entity(EntityManager& entityMng, std::string name) :
	entityMng_(entityMng), name_(name)
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
