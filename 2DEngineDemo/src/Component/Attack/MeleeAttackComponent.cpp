#include "MeleeAttackComponent.h"

#include "../../Constant.h"
#include "../TransformComponent.h"
#include "../../GameObject/Entity.h"

MeleeAttackComponent::MeleeAttackComponent(const std::shared_ptr<Entity>& owner, const int& damage):
	AttackComponent(owner)
{
	damage_ = damage;
}

void MeleeAttackComponent::Initialize()
{
}

void MeleeAttackComponent::Update(const float& deltaTime)
{
	if (lifeTime_ <= 0)
	{
		owner_.lock()->Destroy();
		return;
	}
	lifeTime_ -= deltaTime * second_to_millisecond;
}

void MeleeAttackComponent::Render()
{
	
}
