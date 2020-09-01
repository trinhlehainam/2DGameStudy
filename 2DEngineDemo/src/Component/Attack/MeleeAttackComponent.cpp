#include "MeleeAttackComponent.h"
#include "../TransformComponent.h"
#include "../../GameObject/Entity.h"
#include "../../System/TextureManager.h"
#include "../../System/Camera.h"

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
	
}

void MeleeAttackComponent::Render()
{
	
}
