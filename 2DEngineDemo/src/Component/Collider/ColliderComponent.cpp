#include "ColliderComponent.h"

ColliderComponent::ColliderComponent(const std::shared_ptr<Entity>& owner, std::string tag) :
	owner_(owner), tag_(tag), Component(owner)
{
}

ColliderComponent::ColliderComponent(const std::shared_ptr<Entity>& owner) : owner_(owner), Component(owner)
{
}

void ColliderComponent::SetOffset(const float& X, const float& Y)
{
	offset_.X = X;
	offset_.Y = Y;
}

