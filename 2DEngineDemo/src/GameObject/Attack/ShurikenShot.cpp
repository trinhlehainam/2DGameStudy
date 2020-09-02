#include "ShurikenShot.h"

#include "../../Geometry/Geometry.h"
#include "../../Scene/GameScene.h"

#include "../Entity.h"
#include "../../System/EntityManager.h"
#include "../../System/CollisionManager.h"

#include "../../Component/TransformComponent.h"
#include "../../Component/SpriteComponent.h"
#include "../../Component/Attack/ProjectileEmitterComponent.h"
#include "../../Component/Collider/CircleColliderComponent.h"

namespace
{
	constexpr unsigned int shuriken_width = 20;
	constexpr unsigned int shuriken_height = 20;
	constexpr float scale = 1.0f;
	constexpr float shuriken_radius = shuriken_width * scale / 2.0f;
	constexpr float move_speed = 700.0f;
	constexpr float rotate_speed = 60.0f;
	constexpr float move_range = 1000.0f;
}

ShurikenShot::ShurikenShot(GameScene& gs, const std::shared_ptr<Entity>& owner,const Vector2& startPos,
	const float& angle, const int& damage):
	Attack(gs,owner)
{
	self_ = gs_->entityMng_->AddProjectTile("shuriken");
	self_->AddComponent<TransformComponent>(self_, startPos, shuriken_width, shuriken_height, scale);
	self_->AddComponent<SpriteComponent>(self_);
	auto anim = self_->GetComponent<SpriteComponent>();
	anim->AddAnimation(gs_->GetTexture("shuriken-equip"), "attack",
		Rect(0, 0, shuriken_width, shuriken_height),
		1, rotate_speed);
	anim->PlayLoop("attack");
	auto& collider = gs_->collisionMng_->AddProjectileCollider(self_,
		"PLAYER-SHURIKEN", startPos.X, startPos.Y, shuriken_radius);
	collider.SetOffset(shuriken_radius, shuriken_radius);
	Vector2 velocity = Vector2(move_speed * cosf(angle), move_speed * sinf(angle));
	self_->AddComponent<ProjectileEmitterComponent>(self_, startPos, std::move(velocity), move_range, damage);
}

void ShurikenShot::Initialize()
{
}

void ShurikenShot::SetDamage(const int& damage)
{
	self_->GetComponent<ProjectileEmitterComponent>()->SetDamage(damage);
}

int ShurikenShot::GetDamage() const
{
	return self_->GetComponent<ProjectileEmitterComponent>()->GetDamage();
}

void ShurikenShot::Update(const float& deltaTime)
{
	auto projectile = self_->GetComponent<ProjectileEmitterComponent>();
	if (projectile->IsOutOfRange())
	{
		Vector2 vel = projectile->GetVelocity();
		vel = Vector2(-vel.X, -vel.Y);
		projectile->SetVelocity(vel);
	}
}
