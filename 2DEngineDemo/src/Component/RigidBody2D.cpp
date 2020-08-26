#include "RigidBody2D.h"
#include "TransformComponent.h"
#include "../Game Object/Entity.h"
#include "../System/TextureManager.h"
#include "../System/Camera.h"

RigidBody2D::RigidBody2D(std::shared_ptr<Entity> owner,
	const Vector2& pos, const float& w, const float& h) :
	owner_(owner), collider_(pos, w, h)
{
	desRect_ = collider_;
}

void RigidBody2D::Update(const float& deltaTime)
{
	if (owner_.expired())
	{
		return;
	}
	auto transform = owner_.lock()->GetComponent<TransformComponent>();
	transform->pos += velocity_ * deltaTime;
	collider_.origin.X = transform->pos.X + transform->w * transform->scale / 2.0f - collider_.w / 2;
	collider_.origin.Y = transform->pos.Y + transform->h * transform->scale - collider_.h;
}

void RigidBody2D::Render()
{
	desRect_.origin = collider_.origin - Camera::Instance().viewport.origin;
	desRect_.w = collider_.w;
	desRect_.h = collider_.h;
	TextureManager::DrawBox(desRect_, 0xffffff);
}

bool RigidBody2D::IsOwnerExist()
{
	return !owner_.expired();
}

void RigidBody2D::IsHitBy(ATTACK attackID)
{
	switch (attackID)
	{
	case ATTACK::BOMB:
		owner_.lock()->attackID_ = ATTACK::BOMB;
		break;
	case ATTACK::SHURIKEN:
		owner_.lock()->attackID_ = ATTACK::BOMB;
		break;
	default:
		owner_.lock()->attackID_ = ATTACK::NONE;
		break;
	}
}


