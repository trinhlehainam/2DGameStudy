#include "Slasher.h"

#include "../Entity.h"
#include "../../System/EntityManager.h"
#include "../../Scene/GameScene.h"
#include "../../Component/TransformComponent.h"
#include "../../Component/SpriteComponent.h"
#include "../../Component/RigidBody2D.h"

#include "../../System/AssetManager.h"
#include "../../System/CollisionManager.h"

namespace
{
	const Vector2 start_pos = Vector2(100.0f, 100.0f);
	const Rect src_rect = Rect(0, 0, 32, 32);
	constexpr float slasher_width = 32.0f;
	constexpr float slasher_height = 32.0f;
	constexpr float size_scale = 2.5f;

	constexpr float body_width_scale = 1.3f;
	constexpr float body_heigth_scale = 2.5f;

	constexpr unsigned int run_animation_speed = 100;
	constexpr unsigned int slash_animation_speed = 100;

	constexpr int side_move_velocity = 100;
	constexpr unsigned int slash_distancce = 50;
}

Slasher::Slasher(GameScene& gs, std::shared_ptr<TransformComponent> playerPos):Enemy(gs, playerPos)
{
	
}

Slasher::~Slasher()
{
	rigidBody_ = nullptr;
}

void Slasher::Initialize()
{
	self_ = gs_.entityMng_->AddEntity("slasher");
	self_->AddComponent<TransformComponent>(start_pos, slasher_width, slasher_height, size_scale);
	auto body = gs_.collisionMng_->AddRigidBody2D(
		self_, start_pos, 
		slasher_width * body_width_scale,
		slasher_height * body_heigth_scale
	);
	rigidBody_ = body;
	rigidBody_->tag_ = "SLASHER";
	self_->AddComponent<SpriteComponent>();
	auto anim = self_->GetComponent<SpriteComponent>();
	anim->AddAnimation(gs_.assetMng_->GetTexture("slasher-run"), "run", src_rect, run_animation_speed,3);
	anim->AddAnimation(gs_.assetMng_->GetTexture("slasher-slash"), "slash", src_rect, slash_animation_speed,4);
	anim->Play("run");
	actionUpdate = &Slasher::AimPlayer;
}

std::unique_ptr<Enemy> Slasher::MakeClone()
{
	return std::make_unique<Slasher>(gs_,playerPos_.lock());
}

void Slasher::SetPosition(const Vector2& pos)
{
	auto transform = self_->GetComponent<TransformComponent>();
	transform->pos = pos;
}

void Slasher::Update(const float& deltaTime)
{
	(this->*actionUpdate)(deltaTime);
}

void Slasher::AimPlayer(const float& deltaTime)
{
	auto transform = self_->GetComponent<TransformComponent>();
	auto sprite = self_->GetComponent<SpriteComponent>();

	rigidBody_->velocity_.X = (playerPos_.lock()->pos.X - transform->pos.X) > 0 ? side_move_velocity : -side_move_velocity;

	sprite->isFlipped = rigidBody_->velocity_.X > 0 ? false : true;
	if (std::abs(playerPos_.lock()->pos.X - transform->pos.X) < slash_distancce)
	{
		actionUpdate = &Slasher::SlashUpdate;
		sprite->Play("slash");
		rigidBody_->velocity_.X = 0.0f;
	}
}

void Slasher::SlashUpdate(const float& deltaTime)
{
	auto transform = self_->GetComponent<TransformComponent>();
	auto sprite = self_->GetComponent<SpriteComponent>();
	
	if (std::abs(playerPos_.lock()->pos.X - transform->pos.X) > slash_distancce)
	{
		if (sprite->IsFinished())
		{
			actionUpdate = &Slasher::AimPlayer;
			sprite->Play("run");
		}
	}
}
