#include "BombShot.h"

#include "../../Geometry/Geometry.h"
#include "../../Scene/GameScene.h"

#include "../Entity.h"
#include "../../System/EntityManager.h"
#include "../../System/CollisionManager.h"
#include "../../System/EffectManager.h"
#include "../../System/Camera.h"

#include "../../Component/TransformComponent.h"
#include "../../Component/SpriteComponent.h"
#include "../../Component/Attack/ProjectileEmitterComponent.h"
#include "../../Component/Collider/CircleColliderComponent.h"

namespace
{
	constexpr unsigned int bomb_width = 67;
	constexpr unsigned int bomb_height = 35;
	constexpr float scale = 0.6f;
	constexpr float bomb_radius = bomb_width * scale / 2.0f;
	constexpr float move_speed = 400.0f;
	constexpr int rotate_speed = 20;
	constexpr float move_range = 700.0f;
	constexpr float collider_offset_x = bomb_width * scale / 2.0f;
	constexpr float collider_offset_y = bomb_height * scale / 2.0f;
	constexpr float bomb_exp_scale = 2.0f;

	constexpr int shake_time = 200;
	constexpr float shake_range_x = 10;
	constexpr float shake_range_y = 10;
}

BombShot::BombShot(GameScene& gs, const std::shared_ptr<Entity>& owner, const Vector2& startPos, 
	const float& angle, const int& damage):
	Attack(gs, owner)
{
	self_ = gs_->entityMng_->AddProjectTile("bomb");
	self_->AddComponent<TransformComponent>(self_, startPos, bomb_width, bomb_height, scale);
	self_->AddComponent<SpriteComponent>(self_);
	auto anim = self_->GetComponent<SpriteComponent>();
	anim->AddAnimation(gs_->GetTexture("bomb-equip"), "attack",
		Rect(0, 0, bomb_width, bomb_height),
		1, rotate_speed);
	anim->PlayLoop("attack");
	auto& collider = gs_->collisionMng_->AddProjectileCollider(self_,
		"PLAYER-BOMB", startPos.X, startPos.Y, bomb_radius);
	collider.SetOffset(collider_offset_x, collider_offset_y);

	Vector2 velocity = Vector2(move_speed * cosf(angle), move_speed * sinf(angle));
	self_->AddComponent<ProjectileEmitterComponent>(self_, startPos, std::move(velocity), move_range, damage);
}

void BombShot::Initialize()
{
	
}

void BombShot::SetDamage(const int& damage)
{
	self_->GetComponent<ProjectileEmitterComponent>()->SetDamage(damage);
}

int BombShot::GetDamage() const
{
	return self_->GetComponent<ProjectileEmitterComponent>()->GetDamage();
}

void BombShot::Update(const float& deltaTime)
{
	if (!IsOwnerActive() || self_->GetComponent<ProjectileEmitterComponent>()->IsOutOfRange())
	{
		self_->Destroy();
		auto transform = self_->GetComponent<TransformComponent>();
		Vector2 pos = Vector2(transform->pos.X + transform->w / 2 * transform->scale,
							transform->pos.Y + transform->h / 2 * transform->scale);
		gs_->effectMng_->BombExplosionEffect(pos.X , pos.Y, bomb_exp_scale);
		Camera::Instance().ShakeCamera(shake_time, shake_range_x, shake_range_y);
	}
}
