#include "ProjectileEmitterComponent.h"

#include "../../Constant.h"
#include "../../GameObject/Entity.h"
#include "../../Component/TransformComponent.h"

ProjectileEmitterComponent::ProjectileEmitterComponent(const std::shared_ptr<Entity>& owner, const Vector2& startPos,
	const Vector2& velocity, const float& range, const int& damage):AttackComponent(owner)
{
	start_ = startPos;
	velocity_ = velocity;
	range_ = range;
	damage_ = damage;
}

void ProjectileEmitterComponent::Initialize()
{
}

void ProjectileEmitterComponent::Update(const float& deltaTime)
{
	const auto& owner = owner_.lock();
	const auto& transform = owner->GetComponent<TransformComponent>();
	transform->pos += velocity_ * deltaTime;
	
	if (IsOutOfWorld())
	{
		owner->Destroy();
	}
}

bool ProjectileEmitterComponent::IsOutOfRange()
{
	const auto& owner = owner_.lock();
	const auto& transform = owner->GetComponent<TransformComponent>();
	auto travel_range = transform->pos - start_;
	return travel_range * travel_range >= range_ * range_;
}

bool ProjectileEmitterComponent::IsOutOfWorld()
{
	const auto& owner = owner_.lock();
	const auto& transform = owner->GetComponent<TransformComponent>();
	return transform->pos.X < 0.0f || (transform->pos.X + transform->w) > WORLD_MAP_X ||
		transform->pos.Y < 0.0f || (transform->pos.Y + transform->h) > WORLD_MAP_Y;
}

void ProjectileEmitterComponent::Render()
{
}
