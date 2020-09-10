#include "Slasher.h"

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
	const Rect src_rect = Rect(0, 0, 32, 32);
	constexpr float slasher_width = 32.0f;
	constexpr float slasher_height = 32.0f;
	constexpr float size_scale = 2.5f;

	constexpr float body_width_scale = 1.3f;
	constexpr float body_heigth_scale = 1.6f;
	constexpr float attack_size_x = slasher_width * size_scale;
	constexpr float attack_size_y = 32;
	constexpr int attack_damage = 1;
	constexpr float wait_attack_time = 0.3f;
	constexpr float attack_offset_y = 35;

	constexpr unsigned int run_animation_speed = 100;
	constexpr unsigned int slash_animation_speed = 300;
	constexpr unsigned int hurt_animation_speed = 100;
	constexpr unsigned int death_animation_speed = 200;

	constexpr int normal_side_velocity = 100;
	constexpr unsigned int slash_distancce = 50;
	constexpr float wait_destroy_time = 10.0f;

	constexpr int max_health = 5;
}

Slasher::Slasher(GameScene& gs, const std::shared_ptr<TransformComponent>& playerPos):Enemy(gs, playerPos)
{
	
}

Slasher::~Slasher()
{
	rigidBody_ = nullptr;
}

void Slasher::Initialize()
{
	self_ = gs_.entityMng_->AddEntity("slasher");
	self_->AddComponent<TransformComponent>(self_, start_pos, slasher_width, slasher_height, size_scale);
	self_->AddComponent<SpriteComponent>(self_);
	const auto& anim = self_->GetComponent<SpriteComponent>();
	anim->AddAnimation(gs_.assetMng_->GetTexture("slasher-run"), "run", src_rect, run_animation_speed);
	anim->AddAnimation(gs_.assetMng_->GetTexture("slasher-slash"), "slash", src_rect, slash_animation_speed);
	anim->AddAnimation(gs_.assetMng_->GetTexture("slasher-hurt"), "hurt", src_rect, hurt_animation_speed);
	anim->AddAnimation(gs_.assetMng_->GetTexture("slasher-death"), "death", src_rect, death_animation_speed);
	anim->AddAnimation(gs_.assetMng_->GetTexture("slasher-lying"), "lying", src_rect, 1);
	anim->PlayLoop("run");
	self_->AddComponent<HealthComponent>(self_, max_health);
	actionUpdate_ = &Slasher::AimPlayer;
}

std::unique_ptr<Enemy> Slasher::MakeClone()
{
	return std::make_unique<Slasher>(gs_,playerTransform_.lock());
}

void Slasher::SetPosition(const Vector2& pos)
{
	const auto& transform = self_->GetComponent<TransformComponent>();
	transform->pos = pos;
	// Because RigidBody2D Constructor will Initialize its collider's position depend on owner's Transform
	// Add RigidBody here after set position for owner's transform
	const auto& body = gs_.collisionMng_->AddRigidBody2D(
		self_, start_pos,
		slasher_width * body_width_scale,
		slasher_height * body_heigth_scale
	);
	rigidBody_ = body;
	rigidBody_->SetTag("SLASHER");
}

void Slasher::Update(const float& deltaTime)
{
	(this->*actionUpdate_)(deltaTime);
}

void Slasher::AimPlayer(const float& deltaTime)
{
	CheckHit();

	auto transform = self_->GetComponent<TransformComponent>();
	auto sprite = self_->GetComponent<SpriteComponent>();

	rigidBody_->velocity_.X = (playerTransform_.lock()->pos.X - transform->pos.X) > 0 ? normal_side_velocity : -normal_side_velocity;

	sprite->SetFlipState(!(rigidBody_->velocity_.X > 0));
	if (std::abs(playerTransform_.lock()->pos.X - transform->pos.X) < slash_distancce)
	{
		actionUpdate_ = &Slasher::SlashUpdate;
		sprite->PlayLoop("slash");
		attackFlag_ = true;
		timer_ = wait_attack_time;
		rigidBody_->velocity_.X = 0.0f;
	}
}

void Slasher::SlashUpdate(const float& deltaTime)
{
	CheckHit();

	auto transform = self_->GetComponent<TransformComponent>();
	auto sprite = self_->GetComponent<SpriteComponent>();
	
	if (timer_ <= 0 && attackFlag_)
	{
		attackFlag_ = false;
		timer_ = wait_attack_time;
		Vector2 attack_pos = transform->pos + Vector2(0,attack_offset_y);
		auto melee = gs_.combatMng_->AddAttack<MeleeAttack>(gs_, self_, attack_pos, attack_size_x, attack_size_y);
		melee->SetDamage(attack_damage);
	}

	if (sprite->IsAnimationFinished())
	{
		timer_ = 0;
		actionUpdate_ = &Slasher::AimPlayer;
		sprite->PlayLoop("run");
	}

	timer_ -= deltaTime;
}

void Slasher::HurtUpdate(const float& deltaTime)
{
	auto sprite = self_->GetComponent<SpriteComponent>();
	auto health = self_->GetComponent<HealthComponent>()->Health();
	sprite->PlayOnce("hurt");
	rigidBody_->velocity_.X = 0;
	if (sprite->IsAnimationFinished())
	{
		if (health <= 0)
		{
			actionUpdate_ = &Slasher::DeathUpdate;
			return;
		}
			
		actionUpdate_ = &Slasher::AimPlayer;
		sprite->PlayLoop("run");
	}

}

void Slasher::DeathUpdate(const float& deltaTime)
{
	auto sprite = self_->GetComponent<SpriteComponent>();
	sprite->PlayOnce("death");
	rigidBody_->velocity_.X = 0;
	if (sprite->IsAnimationFinished())
	{
		timer_ = wait_destroy_time;
		sprite->Pause();
		rigidBody_->DeActivate();
		actionUpdate_ = &Slasher::WaitDestroyUpdate;
	}
}

void Slasher::ExplosionDeathUpdate(const float& deltaTime)
{
	gs_.effectMng_->BloodExplosionEffect(rigidBody_->collider_.Center().X, rigidBody_->collider_.Center().Y);
	self_->Destroy();
}

void Slasher::WaitDestroyUpdate(const float& deltaTime)
{
	timer_ -= deltaTime;
	if (timer_ <= 0)
		self_->Destroy();
}

void Slasher::CheckHit()
{
	auto health = self_->GetComponent<HealthComponent>();

	if (health->ReceivedDamage() == health->GetMaxHealth())
	{
		actionUpdate_ = &Slasher::ExplosionDeathUpdate;
		return;
	}

	if (self_->IsHit())
	{
		self_->RecoverHit();
		actionUpdate_ = &Slasher::HurtUpdate;
	}
}
