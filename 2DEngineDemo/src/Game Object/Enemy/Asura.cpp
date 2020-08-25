#include "Asura.h"

#include "../Entity.h"
#include "../EntityManager.h"
#include "../../Scene/GameScene.h"
#include "../../Component/TransformComponent.h"
#include "../../Component/SpriteComponent.h"
#include "../../Component/CircleColliderComponent.h"

#include "../../System/AssetManager.h"
#include "../../System/CollisionManager.h"

namespace {
	const Vector2 start_pos = Vector2(300.0f, 300.0f);
	constexpr float asura_width = 349.0f;
	constexpr float asura_height = 400.0f;
	constexpr float size_scale = 1.0f;
	constexpr float collider_pos_x = 900.0f;
	constexpr float collider_pos_y = 300.0f;
	constexpr float collider_radius = 50.0f;

	constexpr float entering_speed = 100.0f;
	constexpr float ground_line = 250.0f;
	
}

void Asura::EnteringUpdate(const float& deltaTime)
{
	auto transform = self_.lock()->GetComponent<TransformComponent>();
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

Asura::Asura(GameScene& gs, std::shared_ptr<TransformComponent> playerPos_):Enemy(gs,playerPos_)
{
	updater_ = &Asura::EnteringUpdate;
}

Asura::~Asura()
{
}

void Asura::Initialize()
{
	self_ = gs_.entityMng_->AddEntity("asura");
	self_.lock()->AddComponent<TransformComponent>(start_pos, asura_width, asura_height, size_scale);
	self_.lock()->AddComponent<SpriteComponent>();
	auto anim = self_.lock()->GetComponent<SpriteComponent>();
	anim->AddAnimation(gs_.assetMng_->GetTexture("boss-asura"), "idle", Rect(0, 0, asura_width, asura_height), 1, 1);
	anim->Play("idle");
	colliders_ = &gs_.collisionMng_->AddBossCollider(self_.lock(), "asura", collider_pos_x, collider_pos_y, collider_radius);
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
	auto transform = self_.lock()->GetComponent<TransformComponent>();
	transform->pos.X = pos.X - asura_width / 2.0f;
	transform->pos.Y = pos.Y;
}
