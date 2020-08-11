#include "ColliderComponent.h"

#include "../Object/Entity.h"


bool ColliderComponent::IsOwnerExist()
{
	return owner_.expired();
}

ColliderComponent::ColliderComponent(std::shared_ptr<Entity> owner, std::string tag):
	owner_(owner),tag_(tag)
{
	flag_ = true;
}
