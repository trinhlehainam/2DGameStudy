#include "ColliderComponent.h"

#include "../Game Object/Entity.h"


bool ColliderComponent::IsOwnerExist()
{
	return !owner_.expired();
}

std::string ColliderComponent::OwnerTag()
{
	auto ownerTag = owner_.lock()->GetName();
	return ownerTag;
}

ColliderComponent::ColliderComponent(std::shared_ptr<Entity> owner, std::string tag):
	owner_(owner),tag_(tag)
{
	flag_ = true;
}
