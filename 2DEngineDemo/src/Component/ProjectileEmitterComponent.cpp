#include "ProjectileEmitterComponent.h"

#include "../Constant.h"
#include "../Object/Entity.h"
#include "../Component/TransformComponent.h"

namespace
{
	constexpr float PI = 3.14159f;
}

bool ProjectileEmitterComponent::IsOutOfWorld()
{
	auto transform = owner_.GetComponent<TransformComponent>();
	return transform->pos.X < 0.0f || (transform->pos.X + transform->w) > WORLD_MAP_X ||
		transform->pos.Y < 0.0f || (transform->pos.Y + transform->h) > WORLD_MAP_Y;
}

ProjectileEmitterComponent::ProjectileEmitterComponent(Entity& owner, Vector2 startPos, Vector2 velocity, const float& range):
	Component(owner)
{
	start_ = startPos;
	velocity_ = velocity;
	range_ = range;
}

void ProjectileEmitterComponent::Initialize()
{
}

void ProjectileEmitterComponent::Update(const float& deltaTime)
{
	auto transform = owner_.GetComponent<TransformComponent>();
	transform->pos += velocity_ * deltaTime;
	auto travel_range = transform->pos - start_;
	/*if (travel_range * travel_range >= range_ * range_  || IsOutOfWorld())
	{
		owner_.Destroy();
	}*/
}

void ProjectileEmitterComponent::Render()
{
}
