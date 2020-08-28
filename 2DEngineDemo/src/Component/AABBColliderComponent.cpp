#include "AABBColliderComponent.h"

#include "../Game Object/Entity.h"
#include "TransformComponent.h"
#include "../System/TextureManager.h"
#include "../System/Camera.h"

AABBColliderComponent::AABBColliderComponent(std::shared_ptr<Entity> owner, std::string tag,
	const Vector2& pos, const float& w, const float& h):ColliderComponent(owner, tag)
{
	collider_ = Rect(pos, w, h);
	Initialize();
}

AABBColliderComponent::~AABBColliderComponent()
{
}

void AABBColliderComponent::Initialize()
{
	auto transform = owner_.lock()->GetComponent<TransformComponent>();
	collider_.pos = transform->pos;
	destRect_ = collider_;
}

void AABBColliderComponent::Update(const float& deltaTime)
{
	if (owner_.expired())
	{
		return;
	}
	auto transform = owner_.lock()->GetComponent<TransformComponent>();
	collider_.pos = transform->pos + distance_;
}

void AABBColliderComponent::Render()
{
	destRect_.pos = collider_.pos - Camera::Instance().viewport.pos;
	int color = isActive_ ? 0xffff00 : 0xff0000;
	TextureManager::DrawBox(destRect_, color);
}
