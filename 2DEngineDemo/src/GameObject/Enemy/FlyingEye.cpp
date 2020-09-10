#include "FlyingEye.h"

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
	const Rect flight_src_rect = Rect(0, 0, 42, 33);
	const Rect attack_src_rect = Rect(0, 0, 56, 40);
	const Rect hurt_src_rect = Rect(0, 0, 41, 33);
	const Rect death_src_rect = Rect(0, 0, 56, 40);

	constexpr float flyingeye_width = 42;
	constexpr float flyingeye_height = 33;
	constexpr float scale = 1.0f;

	constexpr unsigned int flight_animation_speed = 100;
	constexpr unsigned int attack_animation_speed = 100;
	constexpr unsigned int hurt_animation_speed = 100;
	constexpr unsigned int death_animation_speed = 200;

	constexpr float sight_distance = 150;
	constexpr float attack_distance = 50;
	constexpr float aim_velocity_x = 100;
	constexpr float aim_velocity_y = 100;

	constexpr short int max_health = 5;

	constexpr float wait_destroy_time = 5.0f;
}

FlyingEye::FlyingEye(GameScene& gs, const std::shared_ptr<TransformComponent>& playerPos):
    Enemy(gs,playerPos)
{
}

FlyingEye::~FlyingEye()
{
}

void FlyingEye::Initialize()
{
	self_ = gs_.entityMng_->AddEntity("slasher");
	self_->AddComponent<TransformComponent>(self_, start_pos, flyingeye_width, flyingeye_height, scale);
	self_->AddComponent<SpriteComponent>(self_);
	const auto& anim = self_->GetComponent<SpriteComponent>();
	anim->AddAnimation(gs_.assetMng_->GetTexture("flying-eye-flight"), "flight", flight_src_rect, flight_animation_speed);
	anim->AddAnimation(gs_.assetMng_->GetTexture("flying-eye-attack"), "attack", attack_src_rect, attack_animation_speed);
	anim->AddAnimation(gs_.assetMng_->GetTexture("flying-eye-hurt"), "hurt", hurt_src_rect, hurt_animation_speed);
	anim->AddAnimation(gs_.assetMng_->GetTexture("flying-eye-death"), "death", death_src_rect, death_animation_speed);
	anim->PlayLoop("flight");

	self_->AddComponent<HealthComponent>(self_, max_health);
	actionUpdate_ = &FlyingEye::PatrollingUpdate;
}

void FlyingEye::Update(const float& deltaTime)
{
	rigidBody_->velocity_.Y = 0;
	(this->*actionUpdate_)(deltaTime);
}

std::unique_ptr<Enemy> FlyingEye::MakeClone()
{
    return std::make_unique<FlyingEye>(gs_, playerTransform_.lock());;
}

void FlyingEye::SetPosition(const Vector2& pos)
{
	const auto& transform = self_->GetComponent<TransformComponent>();
	transform->pos = pos;
	// Because RigidBody2D Constructor will Initialize its collider's position depend on owner's Transform
	// Add RigidBody here after set position for owner's transform
	const auto& body = gs_.collisionMng_->AddRigidBody2D(
		self_, start_pos,
		flyingeye_width * scale,
		flyingeye_height * scale
	);
	rigidBody_ = body;
	rigidBody_->SetTag("FLYING-EYE");
}

void FlyingEye::HurtUpdate(const float&)
{
	auto sprite = self_->GetComponent<SpriteComponent>();
	auto health = self_->GetComponent<HealthComponent>()->Health();
	sprite->PlayOnce("hurt");
	rigidBody_->velocity_.X = 0;
	if (sprite->IsAnimationFinished())
	{
		if (health <= 0)
		{
			actionUpdate_ = &FlyingEye::DeathUpdate;
			return;
		}

		actionUpdate_ = &FlyingEye::AimPlayerUpdate;
		sprite->PlayLoop("flight");
	}
}

void FlyingEye::DeathUpdate(const float&)
{
	auto sprite = self_->GetComponent<SpriteComponent>();
	sprite->PlayOnce("death");
	rigidBody_->velocity_.X = 0;
	if (sprite->IsAnimationFinished())
	{
		timer_ = wait_destroy_time;
		sprite->Pause();
		rigidBody_->DeActivate();
		actionUpdate_ = &FlyingEye::WaitForDestroyUpdate;
	}
}

void FlyingEye::PatrollingUpdate(const float&)
{
	CheckHit();

	auto transform = self_->GetComponent<TransformComponent>();
	auto sprite = self_->GetComponent<SpriteComponent>();

	float distance = (playerTransform_.lock()->pos.X - transform->pos.X) * (playerTransform_.lock()->pos.X - transform->pos.X) +
		(playerTransform_.lock()->pos.Y - transform->pos.Y) * (playerTransform_.lock()->pos.Y - transform->pos.Y);
	if (distance <= sight_distance * sight_distance)
	{
		actionUpdate_ = &FlyingEye::AimPlayerUpdate;
	}
}

void FlyingEye::AimPlayerUpdate(const float&)
{
	CheckHit();

	auto transform = self_->GetComponent<TransformComponent>();
	auto sprite = self_->GetComponent<SpriteComponent>();

	rigidBody_->velocity_.X = (playerTransform_.lock()->pos.X - transform->pos.X) > 0 ? aim_velocity_x : -aim_velocity_x;
	rigidBody_->velocity_.Y = (playerTransform_.lock()->pos.Y - transform->pos.Y) > 0 ? aim_velocity_y : -aim_velocity_y;

	sprite->SetFlipState(!(rigidBody_->velocity_.X > 0));
	float distance = (playerTransform_.lock()->pos.X - transform->pos.X) * (playerTransform_.lock()->pos.X - transform->pos.X) +
		(playerTransform_.lock()->pos.Y - transform->pos.Y) * (playerTransform_.lock()->pos.Y - transform->pos.Y);
	if (distance <= attack_distance*attack_distance)
	{
		actionUpdate_ = &FlyingEye::AttackUpdate;
		sprite->PlayOnce("attack");
		rigidBody_->velocity_.X = 0.0f;
	}
}

void FlyingEye::AttackUpdate(const float&)
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
			actionUpdate_ = &FlyingEye::AimPlayerUpdate;
			sprite->PlayLoop("flight");
		}
	}
}

void FlyingEye::WaitForDestroyUpdate(const float& deltaTime)
{
	timer_ -= deltaTime;
	if (timer_ <= 0)
		self_->Destroy();
}

void FlyingEye::CheckHit()
{
	auto health = self_->GetComponent<HealthComponent>();

	if (self_->IsHit())
	{
		self_->RecoverHit();
		actionUpdate_ = &FlyingEye::HurtUpdate;
	}
}