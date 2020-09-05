#include "Asura.h"

#include <algorithm>

#include "../../Constant.h"
#include "../Entity.h"
#include "../Player/Player.h"
#include "../../Scene/GameScene.h"
#include "../../Component/TransformComponent.h"
#include "../../Component/SpriteComponent.h"
#include "../../Component/Collider/CircleColliderComponent.h"
#include "../../Component/HealthComponent.h"

#include "../../System/EntityManager.h"
#include "../../System/AssetManager.h"
#include "../../System/CollisionManager.h"
#include "../../System/EffectManager.h"
#include "../../System/CombatManager.h"
#include "../../System/Camera.h"

#include "../Attack/EnergyBullet.h"

namespace {
	const Vector2 start_pos = Vector2(300.0f, 300.0f);
	constexpr float asura_width = 349.0f;
	constexpr float asura_height = 400.0f;
	constexpr float size_scale = 1.0f;

	constexpr float collider_radius = 25.0f;
	constexpr float collider_offset_x = asura_width / 2;
	constexpr float collider_offset_y = 100.0f;

	constexpr float entering_speed = 100.0f;
	constexpr float ground_line = 350.0f;

	constexpr float emit_ball_cooldown = 1.0f;
	constexpr float wait_attack_time = emit_ball_cooldown;
	constexpr unsigned int emit_ball_play_time = 2000;

	constexpr int energy_ball_num = 3;
	const Vector2 energy_ball_1_offset = Vector2(65, 40);
	const Vector2 energy_ball_2_offset = Vector2(asura_width - 65, 40);
	const Vector2 energy_ball_3_offset = Vector2(asura_width / 2 , asura_height / 2 );
	const Vector2 bullet_offset = Vector2(-35, -10);

	constexpr unsigned int num_ball_each_attack = 8;
	constexpr unsigned int bullet_damage = 1;
	constexpr unsigned int asura_health = 100;
}

Asura::Asura(GameScene& gs, const std::shared_ptr<TransformComponent>& playerPos_) :Enemy(gs, playerPos_)
{
	updater_ = &Asura::EnteringUpdate;
	energyBallPos_.reserve(energy_ball_num);
}

Asura::~Asura()
{
}

void Asura::Initialize()
{
	self_ = gs_.entityMng_->AddEntity("asura");
	self_->AddComponent<TransformComponent>(self_, start_pos, asura_width, asura_height, size_scale);
	self_->AddComponent<SpriteComponent>(self_);
	self_->AddComponent<HealthComponent>(self_, asura_health);
	const auto& anim = self_->GetComponent<SpriteComponent>();
	anim->AddAnimation(gs_.assetMng_->GetTexture("boss-asura"), "idle", Rect(0, 0, asura_width, asura_height), 1);
	anim->PlayLoop("idle");
}

void Asura::EnteringUpdate(const float& deltaTime)
{
	auto transform = self_->GetComponent<TransformComponent>();
	if (transform->pos.Y <= ground_line)
	{
		transform->pos.Y = ground_line;
		Vector2 offset;

		// Add attack's position
		offset = transform->pos + energy_ball_1_offset;
		energyBallPos_.emplace_back(offset);
		offset = transform->pos + energy_ball_2_offset;
		energyBallPos_.emplace_back(offset);
		offset = transform->pos + energy_ball_3_offset;
		energyBallPos_.emplace_back(offset);
		updater_ = &Asura::NormalUpdate;

		// Add Asura's colliders
		auto& collider = gs_.collisionMng_->AddBossCollider(self_, "ASURA", 
			transform->pos.X + collider_offset_x,
			transform->pos.Y + collider_offset_y,
			collider_radius);
		collider->SetOffset(collider_offset_x, collider_offset_y);
		colliders_.push_back(collider);
	}
	else
	{
		transform->pos.Y -= entering_speed * deltaTime;
		Camera::Instance().ShakeCamera(10, 10);
	}
}

void Asura::NormalUpdate(const float& deltaTime)
{
	if (cooldown_ <= 0.0f)
	{
		auto transform = self_->GetComponent<TransformComponent>();
		int energyBallIndex = rand() % energyBallPos_.size();
		auto& energyBall = energyBallPos_[energyBallIndex];
		gs_.effectMng_->EnergyBallEffect(emit_ball_play_time, energyBall.X, energyBall.Y);
		// random floating-point number
		float angle = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * PI;
		attacks_.emplace_back(energyBall + bullet_offset, angle, wait_attack_time);
		cooldown_ = emit_ball_cooldown;
	}

	for (auto& attack : attacks_)
	{
		attack.waitTimer -= deltaTime;
		if (attack.waitTimer <= 0.0f)
		{
			for (int i = 0; i < num_ball_each_attack; ++i)
			{
				auto bullet = gs_.combatMng_->AddAttack<EnergyBullet>(gs_, self_, attack.pos, attack.angle);
				bullet->SetDamage(bullet_damage);
				attack.angle += PI / 4.0f;
			}
			attack.flag = false;
		}
		attack.waitTimer -= deltaTime;
	}

	attacks_.erase(std::remove_if(attacks_.begin(), attacks_.end(), [](AsuraAttack& attack) {
		return !attack.flag; }),
		attacks_.end());

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
	self_->Destroy();
	Camera::Instance().FollowEntityMode();
	auto playerTransform = playerTransform_.lock();
	playerTransform->SetLeftLimit(0);
	playerTransform->SetRightLimit(WORLD_MAP_X - playerTransform->w);
}

void Asura::CheckHit()
{
	auto healthComponent = self_->GetComponent<HealthComponent>();
	auto health = healthComponent->Health();
	if (health <= 0)
	{
		updater_ = &Asura::DeadUpdate;
	}
}

void Asura::Update(const float& deltaTime)
{
	CheckHit();
	(this->*updater_)(deltaTime);
}

std::unique_ptr<Enemy> Asura::MakeClone()
{
	return std::make_unique<Asura>(gs_, playerTransform_.lock());
}

void Asura::SetPosition(const Vector2& pos)
{
	const auto& transform = self_->GetComponent<TransformComponent>();
	transform->pos.X = pos.X - asura_width / 2.0f;
	transform->pos.Y = pos.Y;
}
