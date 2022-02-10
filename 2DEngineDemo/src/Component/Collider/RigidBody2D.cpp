#include "RigidBody2D.h"
#include "../TransformComponent.h"
#include "../../GameObject/Entity.h"
#include "../../System/TextureManager.h"
#include "../../System/Camera.h"

RigidBody2D::RigidBody2D(const std::shared_ptr<Entity>& owner,
	const Vector2& pos, const float& w, const float& h) :
	ColliderComponent(owner),collider_(pos, w, h)
{
	auto transform = owner_.lock()->GetComponent<TransformComponent>();
	collider_.pos.X = transform->pos.X + transform->w * transform->scale / 2.0f - collider_.w / 2;
	collider_.pos.Y = transform->pos.Y + transform->h * transform->scale - collider_.h;
	destRect_ = collider_;
	updater_ = &RigidBody2D::NormalUpdate;
}

void RigidBody2D::NormalUpdate(const float& deltaTime)
{
	velocity_.X = clamp(velocity_.X, -maxVelocity_.X, maxVelocity_.X);
	velocity_.Y = clamp(velocity_.Y, -maxVelocity_.Y, maxVelocity_.Y);
	collider_.pos += velocity_ * deltaTime;
}

void RigidBody2D::Update(const float& deltaTime)
{
	if (owner_.expired())
		return;

	auto transform = owner_.lock()->GetComponent<TransformComponent>();

	(this->*updater_)(deltaTime);

	transform->UpdateLimitPosition(collider_.pos.X, collider_.pos.Y);
	transform->pos.X = collider_.pos.X + collider_.w / 2.0f - transform->w * transform->scale / 2.0f;
	transform->pos.Y = collider_.pos.Y + collider_.h - transform->h * transform->scale;
}

void RigidBody2D::ImpactUpdate(const float& deltaTime)
{
	collider_.pos.X += impactVeloctity_.X * deltaTime;
	collider_.pos.Y += velocity_.Y * deltaTime;
	if (impactVeloctity_.X < 0)
	{
		impactVeloctity_.X += impactResist_.X * deltaTime;
		if (impactVeloctity_.X >= 0)
			impactVeloctity_.X = 0;

	}
	else if (impactVeloctity_.X > 0)
	{
		impactVeloctity_.X -= impactResist_.X * deltaTime;
		if (impactVeloctity_.X <= 0)
			impactVeloctity_.X = 0;
	}
	else
	{
		updater_ = &RigidBody2D::NormalUpdate;
		velocity_.X = 0;
	}
		
}

void RigidBody2D::Initialize()
{
}

void RigidBody2D::Render()
{
	destRect_.pos = collider_.pos - Camera::Instance().Position();
	destRect_.w = collider_.w;
	destRect_.h = collider_.h;
	TextureManager::DrawDebugBox(destRect_, 0xffffff);
}

void RigidBody2D::SetMaxVelocity(const float& velX, const float& velY)
{
	maxVelocity_.X = velX;
	maxVelocity_.Y = velY;
}

void RigidBody2D::Impact(const Vector2& impactValue)
{
	impactVeloctity_ = impactValue;
	updater_ = &RigidBody2D::ImpactUpdate;
}



