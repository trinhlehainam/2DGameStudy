#include "ColliderComponent.h"

#include "../GameObject/Entity.h"


bool ColliderComponent::IsOwnerExist()
{
	return !owner_.expired();
}

std::string ColliderComponent::OwnerTag()
{
	auto ownerTag = owner_.lock()->GetName();
	return ownerTag;
}

void ColliderComponent::SetDistance(const float& X, const float& Y)
{
	distance_.X = X;
	distance_.Y = Y;
}

ColliderComponent::ColliderComponent(std::shared_ptr<Entity> owner, std::string tag):
	owner_(owner),tag_(tag)
{
}
