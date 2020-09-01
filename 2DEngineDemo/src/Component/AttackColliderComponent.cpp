#include "AttackColliderComponent.h"
#include "TransformComponent.h"
#include "../GameObject/Entity.h"
#include "../System/TextureManager.h"
#include "../System/Camera.h"

AttackColliderComponent::AttackColliderComponent(std::shared_ptr<Entity> owner, const Vector2& pos, const float& w, const float& h) :
	ColliderComponent(owner), collider_(pos, w, h)
{
	auto transform = owner_.lock()->GetComponent<TransformComponent>();
	collider_.pos = transform->pos;
	destRect_ = collider_;
}

void AttackColliderComponent::Initialize()
{
}

void AttackColliderComponent::Update(const float& deltaTime)
{
	if (owner_.expired())
	{
		return;
	}
	const auto& transform = owner_.lock()->GetComponent<TransformComponent>();
	collider_.pos = transform->pos + distance_;
}

void AttackColliderComponent::Render()
{
	destRect_.pos = collider_.pos - Camera::Instance().viewport.pos;
	int color = isActive_ ? 0xffff00 : 0xff0000;
	TextureManager::DrawDebugBox(destRect_, color);
}
