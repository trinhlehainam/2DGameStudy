#include "Mushroom.h"

#include "../Entity.h"

#include "../../Scene/GameScene.h"

#include "../../Component/TransformComponent.h"
#include "../../Component/SpriteComponent.h"
#include "../../Component/Collider/RigidBody2D.h"
#include "../../Component/HealthComponent.h"
#include "../Attack/MeleeAttack.h"

#include "../../System/AssetManager.h"
#include "../../System/CollisionManager.h"
#include "../../System/EntityManager.h"
#include "../../System/EffectManager.h"
#include "../../System/CombatManager.h"

namespace
{
	const Vector2 start_pos = Vector2(100.0f, 100.0f);
	const Rect src_rect = Rect(0, 0, 64, 48);

	constexpr float mushroom_width = 64;
	constexpr float mushroom_height = 48;
	constexpr float scale = 1.5f;
	constexpr float rigidbody_width_scale = 0.6f;

	constexpr unsigned int run_animation_speed = 100;
	constexpr unsigned int attack_animation_speed = 100;
	constexpr unsigned int hurt_animation_speed = 100;
	constexpr unsigned int death_animation_speed = 300;
	constexpr unsigned int idle_animation_speed = 100;

	constexpr float sight_distance = 150;
	constexpr float attack_distance = 50;
	constexpr float aim_velocity_x = 100;
	constexpr float guard_velocity_x = 50;

	constexpr float attack_size_x = 26 * scale;
	constexpr float attack_size_y = 48 * scale;
	constexpr int attack_damage = 1;
	constexpr float wait_attack_time = 0.5f;
	constexpr float attack_right_offset_x = 50;
	constexpr float attack_left_offset_x = 10;

	constexpr short int max_health = 10;

	constexpr float wait_destroy_time = 5.0f;
	constexpr float guard_area_x = 300;
	constexpr float moving_offset = 5;
}

Mushroom::Mushroom(GameScene& gs, const std::shared_ptr<TransformComponent>& playerPos) :
	Enemy(gs, playerPos)
{
}

Mushroom::~Mushroom()
{
}

void Mushroom::Initialize()
{
	self_ = gs_.entityMng_->AddEntity("slasher");
	self_->AddComponent<TransformComponent>(self_, start_pos, mushroom_width, mushroom_height, scale);
	self_->AddComponent<SpriteComponent>(self_);
	const auto& anim = self_->GetComponent<SpriteComponent>();
	anim->AddAnimation(gs_.assetMng_->GetTexture("mushroom-Attack"), "attack", src_rect, attack_animation_speed);
	anim->AddAnimation(gs_.assetMng_->GetTexture("mushroom-Death"), "death", src_rect, death_animation_speed);
	anim->AddAnimation(gs_.assetMng_->GetTexture("mushroom-Hurt"), "hurt", src_rect, hurt_animation_speed);
	anim->AddAnimation(gs_.assetMng_->GetTexture("mushroom-Idle"), "idle", src_rect, idle_animation_speed);
	anim->AddAnimation(gs_.assetMng_->GetTexture("mushroom-Run"), "run", src_rect, run_animation_speed);
	anim->PlayLoop("run");

	self_->AddComponent<HealthComponent>(self_, max_health);
	actionUpdate_ = &Mushroom::PatrollingUpdate;
}

void Mushroom::Update(const float& deltaTime)
{
	(this->*actionUpdate_)(deltaTime);
}

std::unique_ptr<Enemy> Mushroom::MakeClone()
{
	return std::make_unique<Mushroom>(gs_, playerTransform_.lock());;
}

void Mushroom::SetPosition(const Vector2& pos)
{
	const auto& transform = self_->GetComponent<TransformComponent>();
	transform->pos = pos;
	startPosX_ = pos.X;
	// Because RigidBody2D Constructor will Initialize its collider's position depend on owner's Transform
	// Add RigidBody here after set position for owner's transform
	const auto& body = gs_.collisionMng_->AddRigidBody2D(
		self_, start_pos,
		mushroom_width * rigidbody_width_scale,
		mushroom_height * scale
	);
	rigidBody_ = body;
	rigidBody_->SetTag("MUSHROOM");
}

void Mushroom::HurtUpdate(const float&)
{
	auto sprite = self_->GetComponent<SpriteComponent>();
	auto health = self_->GetComponent<HealthComponent>()->Health();
	sprite->PlayOnce("hurt");
	rigidBody_->velocity_.X = 0;
	if (sprite->IsAnimationFinished())
	{
		if (health <= 0)
		{
			actionUpdate_ = &Mushroom::DeathUpdate;
			return;
		}

		actionUpdate_ = &Mushroom::AimPlayerUpdate;
		sprite->PlayLoop("run");
	}
}

void Mushroom::DeathUpdate(const float&)
{
	auto sprite = self_->GetComponent<SpriteComponent>();
	sprite->PlayOnce("death");
	rigidBody_->velocity_.X = 0;
	if (sprite->IsAnimationFinished() && rigidBody_->isGrounded_)
	{
		timer_ = wait_destroy_time;
		sprite->Pause();
		rigidBody_->DeActivate();
		actionUpdate_ = &Mushroom::WaitForDestroyUpdate;
	}
}

void Mushroom::PatrollingUpdate(const float&)
{
	CheckHit();

	auto transform = self_->GetComponent<TransformComponent>();
	auto sprite = self_->GetComponent<SpriteComponent>();
	auto& playerPos = playerTransform_.lock()->pos;

	if (transform->pos.X - startPosX_ >= guard_area_x)
		rigidBody_->velocity_.X = -guard_velocity_x;
	else if (transform->pos.X - startPosX_ <= 0)
		rigidBody_->velocity_.X = guard_velocity_x;

	sprite->SetFlipState(!(rigidBody_->velocity_.X > 0));

	float distance = (playerPos.X - transform->pos.X) * (playerPos.X - transform->pos.X) +
		(playerPos.Y - transform->pos.Y) * (playerPos.Y - transform->pos.Y);
	if (distance <= sight_distance * sight_distance)
	{
		actionUpdate_ = &Mushroom::AimPlayerUpdate;
	}
}

void Mushroom::AimPlayerUpdate(const float&)
{
	CheckHit();

	auto transform = self_->GetComponent<TransformComponent>();
	auto sprite = self_->GetComponent<SpriteComponent>();
	auto& playerPos = playerTransform_.lock()->pos;

	if ((playerPos.X - transform->pos.X) > moving_offset)
		rigidBody_->velocity_.X = aim_velocity_x;
	else if ((playerPos.X - transform->pos.X) < -moving_offset)
		rigidBody_->velocity_.X = -aim_velocity_x;
	else
		rigidBody_->velocity_.X = 0;

	sprite->SetFlipState(!(rigidBody_->velocity_.X > 0));
	float distance = (playerTransform_.lock()->pos.X - transform->pos.X) * (playerTransform_.lock()->pos.X - transform->pos.X) +
		(playerTransform_.lock()->pos.Y - transform->pos.Y) * (playerTransform_.lock()->pos.Y - transform->pos.Y);
	if (distance <= attack_distance * attack_distance)
	{
		actionUpdate_ = &Mushroom::AttackUpdate;
		sprite->PlayOnce("attack");
		timer_ = wait_attack_time;
		attackFlag_ = true;
		rigidBody_->velocity_.X = 0.0f;
	}
}

void Mushroom::AttackUpdate(const float& deltaTime)
{
	CheckHit();

	auto transform = self_->GetComponent<TransformComponent>();
	auto sprite = self_->GetComponent<SpriteComponent>();

	if (timer_ <= 0 && attackFlag_)
	{
		attackFlag_ = false;
		timer_ = wait_attack_time;
		Vector2 attack_pos;
		attack_pos.Y = transform->pos.Y;
		attack_pos.X = !sprite->IsFlipped() ? transform->pos.X + attack_right_offset_x : 
			transform->pos.X - attack_left_offset_x;
		auto melee = gs_.combatMng_->AddAttack<MeleeAttack>(gs_, self_, attack_pos, attack_size_x, attack_size_y);
		melee->SetDamage(attack_damage);
	}

	if (sprite->IsAnimationFinished())
	{
		sprite->PlayLoop("run");
		actionUpdate_ = &Mushroom::AimPlayerUpdate;
	}

	timer_ -= deltaTime;
}

void Mushroom::WaitForDestroyUpdate(const float& deltaTime)
{
	timer_ -= deltaTime;
	if (timer_ <= 0)
		self_->Destroy();
}

void Mushroom::CheckHit()
{
	auto health = self_->GetComponent<HealthComponent>();

	if (self_->IsHit())
	{
		self_->RecoverHit();
		actionUpdate_ = &Mushroom::HurtUpdate;
	}
}