#include "RigidBody2D.h"
#include "TransformComponent.h"
#include "../GameObject/Entity.h"
#include "../System/TextureManager.h"
#include "../System/Camera.h"

RigidBody2D::RigidBody2D(const std::shared_ptr<Entity>& owner,
	const Vector2& pos, const float& w, const float& h) :
	ColliderComponent(owner),collider_(pos, w, h)
{
	auto transform = owner_.lock()->GetComponent<TransformComponent>();
	collider_.pos.X = transform->pos.X + transform->w * transform->scale / 2.0f - collider_.w / 2;
	collider_.pos.Y = transform->pos.Y + transform->h * transform->scale - collider_.h;
	destRect_ = collider_;
}

void RigidBody2D::Initialize()
{
}

void RigidBody2D::Update(const float& deltaTime)
{
	if (owner_.expired())
	{
		return;
	}
	auto transform = owner_.lock()->GetComponent<TransformComponent>();
	collider_.pos += velocity_ * deltaTime;
	transform->pos.X = collider_.pos.X + collider_.w / 2.0f - transform->w * transform->scale / 2.0f;
	transform->pos.Y = collider_.pos.Y + collider_.h - transform->h * transform->scale;
}

void RigidBody2D::Render()
{
	destRect_.pos = collider_.pos - Camera::Instance().viewport.pos;
	destRect_.w = collider_.w;
	destRect_.h = collider_.h;
	TextureManager::DrawDebugBox(destRect_, 0xffffff);
}



