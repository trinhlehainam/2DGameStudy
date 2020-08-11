#include "ColliderComponent.h"


ColliderComponent::ColliderComponent(std::shared_ptr<TransformComponent> transfrom, std::string tag):
	transform_(transfrom),tag_(tag)
{
	flag_ = true;
}
