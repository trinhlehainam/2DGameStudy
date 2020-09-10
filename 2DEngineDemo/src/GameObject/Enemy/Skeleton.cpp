#include "Skeleton.h"

#include "../Entity.h"

#include "../../Scene/GameScene.h"

#include "../../Component/TransformComponent.h"
#include "../../Component/SpriteComponent.h"
#include "../../Component/Collider/RigidBody2D.h"
#include "../../Component/HealthComponent.h"

#include "../../System/AssetManager.h"
#include "../../System/CollisionManager.h"
#include "../../System/EntityManager.h"
#include "../../System/EffectManager.h"

namespace
{
	const Vector2 start_pos = Vector2(100.0f, 100.0f);
	const Rect death_src_rect = Rect(0, 0, 56, 57);
	const Rect hurt_src_rect = Rect(0, 0, 55, 57);
	const Rect attack_src_rect = Rect(0, 0, 95, 57);
	const Rect idle_src_rect = Rect(0, 0, 48, 57);
	const Rect run_src_rect = Rect(0, 0, 48, 57);
	const Vector2 attack_anim_offset = Vector2(10, 0);

	constexpr float skeleton_width = 48;
	constexpr float skeleton_height = 57;
	constexpr float scale = 1.5f;
	constexpr float rigidbody_width_scale = 0.6f;

	constexpr unsigned int run_animation_speed = 100;
	constexpr unsigned int attack_animation_speed = 100;
	constexpr unsigned int hurt_animation_speed = 100;
	constexpr unsigned int death_animation_speed = 300;
	constexpr unsigned int idle_animation_speed = 100;

	constexpr float sight_distance = 150;
	constexpr float attack_distance = 80;
	constexpr float aim_velocity_x = 100;
	constexpr float guard_velocity_x = 50;

	constexpr short int max_health = 12;

	constexpr float wait_destroy_time = 5.0f;
	constexpr float guard_area_x = 300;
	constexpr float moving_offset = 5;
}

Skeleton::Skeleton(GameScene& gs, const std::shared_ptr<TransformComponent>& playerPos) :
	Enemy(gs, playerPos)
{
}

Skeleton::~Skeleton()
{
}

void Skeleton::Initialize()
{
	self_ = gs_.entityMng_->AddEntity("slasher");
	self_->AddComponent<TransformComponent>(self_, start_pos, skeleton_width, skeleton_height, scale);
	self_->AddComponent<SpriteComponent>(self_);
	const auto& anim = self_->GetComponent<SpriteComponent>();
	anim->AddAnimation(gs_.assetMng_->GetTexture("skeleton-Attack"), "attack", attack_src_rect, attack_animation_speed);
	anim->AddAnimation(gs_.assetMng_->GetTexture("skeleton-Death"), "death", death_src_rect, death_animation_speed);
	anim->AddAnimation(gs_.assetMng_->GetTexture("skeleton-Hurt"), "hurt", hurt_src_rect, hurt_animation_speed);
	anim->AddAnimation(gs_.assetMng_->GetTexture("skeleton-Idle"), "idle", idle_src_rect, idle_animation_speed);
	anim->AddAnimation(gs_.assetMng_->GetTexture("skeleton-Run"), "run", run_src_rect, run_animation_speed);
	anim->PlayLoop("run");

	anim->SetAnimationOffset("attack", attack_anim_offset);

	self_->AddComponent<HealthComponent>(self_, max_health);
	actionUpdate_ = &Skeleton::PatrollingUpdate;
}

void Skeleton::Update(const float& deltaTime)
{
	(this->*actionUpdate_)(deltaTime);
}

std::unique_ptr<Enemy> Skeleton::MakeClone()
{
	return std::make_unique<Skeleton>(gs_, playerTransform_.lock());;
}

void Skeleton::SetPosition(const Vector2& pos)
{
	const auto& transform = self_->GetComponent<TransformComponent>();
	transform->pos = pos;
	startPosX_ = pos.X;
	// Because RigidBody2D Constructor will Initialize its collider's position depend on owner's Transform
	// Add RigidBody here after set position for owner's transform
	const auto& body = gs_.collisionMng_->AddRigidBody2D(
		self_, start_pos,
		skeleton_width * rigidbody_width_scale,
		skeleton_height * scale
	);
	rigidBody_ = body;
	rigidBody_->SetTag("MUSHROOM");
}

void Skeleton::HurtUpdate(const float&)
{
	auto sprite = self_->GetComponent<SpriteComponent>();
	auto health = self_->GetComponent<HealthComponent>()->Health();
	sprite->PlayOnce("hurt");
	rigidBody_->velocity_.X = 0;
	if (sprite->IsAnimationFinished())
	{
		if (health <= 0)
		{
			actionUpdate_ = &Skeleton::DeathUpdate;
			return;
		}

		actionUpdate_ = &Skeleton::AimPlayerUpdate;
		sprite->PlayLoop("run");
	}
}

void Skeleton::DeathUpdate(const float&)
{
	auto sprite = self_->GetComponent<SpriteComponent>();
	sprite->PlayOnce("death");
	rigidBody_->velocity_.X = 0;
	if (sprite->IsAnimationFinished() && rigidBody_->isGrounded_)
	{
		timer_ = wait_destroy_time;
		sprite->Pause();
		rigidBody_->DeActivate();
		actionUpdate_ = &Skeleton::WaitForDestroyUpdate;
	}
}

void Skeleton::PatrollingUpdate(const float&)
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
		actionUpdate_ = &Skeleton::AimPlayerUpdate;
	}
}

void Skeleton::AimPlayerUpdate(const float&)
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
		actionUpdate_ = &Skeleton::AttackUpdate;
		sprite->PlayLoop("attack");
		rigidBody_->velocity_.X = 0.0f;
	}
}

void Skeleton::AttackUpdate(const float&)
{
	CheckHit();

	auto transform = self_->GetComponent<TransformComponent>();
	auto sprite = self_->GetComponent<SpriteComponent>();

	float distance = (playerTransform_.lock()->pos.X - transform->pos.X) * (playerTransform_.lock()->pos.X - transform->pos.X) +
		(playerTransform_.lock()->pos.Y - transform->pos.Y) * (playerTransform_.lock()->pos.Y - transform->pos.Y);
	if (distance > attack_distance * attack_distance)
	{
		if (sprite->IsAnimationFinished())
		{
			sprite->PlayLoop("run");
			actionUpdate_ = &Skeleton::AimPlayerUpdate;
		}
	}
}

void Skeleton::WaitForDestroyUpdate(const float& deltaTime)
{
	timer_ -= deltaTime;
	if (timer_ <= 0)
		self_->Destroy();
}

void Skeleton::CheckHit()
{
	auto health = self_->GetComponent<HealthComponent>();

	if (self_->IsHit())
	{
		self_->RecoverHit();
		actionUpdate_ = &Skeleton::HurtUpdate;
	}
}