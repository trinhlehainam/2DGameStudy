#include "AABBColliderComponent.h"

#include "TransformComponent.h"
#include "../System/TextureManager.h"
#include "../System/Camera.h"

AABBColliderComponent::AABBColliderComponent(std::shared_ptr<TransformComponent> transform, std::string tag,
	const Vector2& pos, const float& w, const float& h):ColliderComponent(transform, tag)
{
	collider_ = Rect(pos, w, h);
	Initialize();
}

AABBColliderComponent::~AABBColliderComponent()
{
}

void AABBColliderComponent::Initialize()
{
	auto transform = transform_.lock();
	collider_.origin = transform->pos;
	destRect_ = collider_;
}

void AABBColliderComponent::Update(const float& deltaTime)
{
	auto transform = transform_.lock();
	collider_.origin = transform->pos;
	
}

void AABBColliderComponent::Render()
{
	destRect_.origin = collider_.origin - Camera::Instance().viewport.origin;
	int color = flag_ ? 0xffff00 : 0xff0000;
	TextureManager::DrawBox(destRect_, color);
}
