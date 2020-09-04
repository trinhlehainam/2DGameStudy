#include "Asura.h"

#include "../Entity.h"
#include "../../System/EntityManager.h"
#include "../../Scene/GameScene.h"
#include "../../Component/TransformComponent.h"
#include "../../Component/SpriteComponent.h"
#include "../../Component/Collider/CircleColliderComponent.h"

#include "../../System/AssetManager.h"
#include "../../System/CollisionManager.h"
#include "../../System/EffectManager.h"

namespace {
	const Vector2 start_pos = Vector2(300.0f, 300.0f);
	constexpr float asura_width = 349.0f;
	constexpr float asura_height = 400.0f;
	constexpr float size_scale = 1.0f;
	constexpr float collider_pos_x = 900.0f;
	constexpr float collider_pos_y = 300.0f;
	constexpr float collider_radius = 50.0f;

	constexpr float entering_speed = 100.0f;
	constexpr float ground_line = 350.0f;

	constexpr float emit_ball_cooldown = 1.0f;
	constexpr unsigned int emit_ball_play_time = 500;
}

Asura::Asura(GameScene& gs, const std::shared_ptr<TransformComponent>& playerPos_) :Enemy(gs, playerPos_)
{
	updater_ = &Asura::EnteringUpdate;
}

Asura::~Asura()
{
}

void Asura::Initialize()
{
	self_ = gs_.entityMng_->AddEntity("asura");
	self_->AddComponent<TransformComponent>(self_, start_pos, asura_width, asura_height, size_scale);
	self_->AddComponent<SpriteComponent>(self_);
	const auto& anim = self_->GetComponent<SpriteComponent>();
	anim->AddAnimation(gs_.assetMng_->GetTexture("boss-asura"), "idle", Rect(0, 0, asura_width, asura_height), 1);
	anim->PlayLoop("idle");
	auto& collider = gs_.collisionMng_->AddBossCollider(self_, "asura", collider_pos_x, collider_pos_y, collider_radius);
	colliders_.push_back(collider);
}

void Asura::EnteringUpdate(const float& deltaTime)
{
	auto transform = self_->GetComponent<TransformComponent>();
	if (transform->pos.Y <= ground_line)
	{
		transform->pos.Y = ground_line;
		updater_ = &Asura::NormalUpdate;
	}
	else
		transform->pos.Y -= entering_speed * deltaTime;
}

void Asura::NormalUpdate(const float& deltaTime)
{
	if (cooldown_ <= 0.0f)
	{
		auto transform = self_->GetComponent<TransformComponent>();
		gs_.effectMng_->EnergyBallEffect(emit_ball_play_time, transform->pos.X, transform->pos.Y);
		cooldown_ = emit_ball_cooldown;
	}
	cooldown_ -= deltaTime;
}

void Asura::DamageUpdate(const float& deltaTime)
{
}

void Asura::ExitingUpdate(const float& deltaTime)
{
}

void Asura::DeadUpdate(const float& deltaTime)
{
}

void Asura::Update(const float& deltaTime)
{
	(this->*updater_)(deltaTime);
}

std::unique_ptr<Enemy> Asura::MakeClone()
{
	return std::make_unique<Asura>(gs_, playerPos_.lock());
}

void Asura::SetPosition(const Vector2& pos)
{
	const auto& transform = self_->GetComponent<TransformComponent>();
	transform->pos.X = pos.X - asura_width / 2.0f;
	transform->pos.Y = pos.Y;
}
