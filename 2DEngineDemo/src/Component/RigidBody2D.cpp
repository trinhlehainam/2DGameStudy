#include "RigidBody2D.h"
#include "TransformComponent.h"
#include "../System/TextureManager.h"
#include "../System/Camera.h"

RigidBody2D::RigidBody2D(std::shared_ptr<TransformComponent> transform,
	const Vector2& pos, const float& w, const float& h) :
	transform_(transform), collider_(pos, w, h)
{
	desRect_ = collider_;
}

void RigidBody2D::SetVelocity(const Vector2& velocity)
{
}

void RigidBody2D::Update(const float& deltaTime)
{
	auto transform = transform_.lock();
	transform->pos += velocity_ * deltaTime;
	collider_.origin.X = transform->pos.X + transform->w * transform->scale / 2.0f - collider_.w / 2;
	collider_.origin.Y = transform->pos.Y;
	
}

void RigidBody2D::Render()
{
	desRect_.origin = collider_.origin - Camera::Instance().viewport.origin;
}


