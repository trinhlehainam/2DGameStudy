#include "EnergyBullet.h"

#include "../../Scene/GameScene.h"

#include "../Entity.h"
#include "../../System/EntityManager.h"
#include "../../System/PhysicsManager.h"
#include "../../System/EffectManager.h"

#include "../../Component/TransformComponent.h"
#include "../../Component/SpriteComponent.h"
#include "../../Component/Attack/ProjectileEmitterComponent.h"
#include "../../Component/Collider/CircleColliderComponent.h"

namespace
{
	constexpr unsigned int energy_bullet_width = 64;
	constexpr unsigned int energy_bullet_height = 32;
	constexpr unsigned int energy_bullet_anima_speed = 100;
	constexpr float scale = 1.0f;
	constexpr float energy_bullet_radius = energy_bullet_height * scale / 2.0f;
	constexpr float collider_offset_x = energy_bullet_width * scale / 2.0f;
	constexpr float collider_offset_y = energy_bullet_height * scale / 2.0f;
	constexpr float move_speed = 300.0f;
	constexpr float move_range = 500.0f;
}

EnergyBullet::EnergyBullet(GameScene& gs, const std::shared_ptr<Entity>& owner, const Vector2& startPos,
	const float& angle, const int& damage):Attack(gs,owner)
{
	self_ = gs_->entityMng_->AddProjectTile("energy-bullet");
	self_->AddComponent<TransformComponent>(self_, startPos, energy_bullet_width, energy_bullet_height, scale);
	self_->AddComponent<SpriteComponent>(self_);
	auto anim = self_->GetComponent<SpriteComponent>();
	anim->AddAnimation(gs_->GetTexture("energy-bullet"), "attack",
		Rect(0, 0, energy_bullet_width, energy_bullet_height),
		 energy_bullet_anima_speed, angle);
	anim->PlayLoop("attack");
	auto& collider = gs_->physicsMng_->AddProjectileCollider(self_,
		"ASURA-BULLET", startPos.X, startPos.Y, energy_bullet_radius);
	collider.SetOffset(collider_offset_x, collider_offset_y);
	Vector2 velocity = Vector2(move_speed * cosf(angle), move_speed * sinf(angle));
	self_->AddComponent<ProjectileEmitterComponent>(self_, startPos, std::move(velocity), move_range, damage);
}

void EnergyBullet::Initialize()
{
	
}

void EnergyBullet::SetDamage(const int& damage)
{
	self_->GetComponent<ProjectileEmitterComponent>()->SetDamage(damage);
}

int EnergyBullet::GetDamage() const
{
	return self_->GetComponent<ProjectileEmitterComponent>()->GetDamage();
}

void EnergyBullet::Update(const float& deltaTime)
{
	if (!IsOwnerActive() || self_->GetComponent<ProjectileEmitterComponent>()->IsOutOfRange())
	{
		self_->Destroy();
		auto transform = self_->GetComponent<TransformComponent>();
		Vector2 pos = Vector2(transform->pos.X + transform->w / 2 * transform->scale,
			transform->pos.Y + transform->h / 2 * transform->scale);
		gs_->effectMng_->EliminateEnergyBulletEffect(pos.X, pos.Y);
	}

}
