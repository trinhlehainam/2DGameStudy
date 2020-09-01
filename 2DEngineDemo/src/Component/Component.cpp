#include "Component.h"

#include "../GameObject/Entity.h"

Component::Component(const std::shared_ptr<Entity>& owner):owner_(owner)
{
}

std::string Component::GetOwnerName()
{
	return owner_.lock()->GetName();
}
