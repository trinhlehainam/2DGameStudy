#include "Player.h"

#include "../../Scene/GameScene.h"
#include "../../System/AssetManager.h"
#include "../../System/Camera.h"
#include "../../System/CollisionManager.h"

#include "../Entity.h"
#include "../../System/EntityManager.h"
#include "../Player/ShurikenEquip.h"
#include "../Player/BombEquip.h"

#include "../../Component/TransformComponent.h"
#include "../../Component/RigidBody2D.h"
#include "../../Component/SpriteComponent.h"

#include "../../Geometry/Geometry.h"

#include "../../Input/KeyboardInput.h"


namespace
{
	const Vector2 start_pos = Vector2(30.0f, 500.0f);
	constexpr float player_width = 32.0f;
	constexpr float player_height = 32.0f;
	constexpr float player_scale = 2.0f;

	constexpr float rigidbody_width_scale = 1.2f;
	constexpr float rigidbody_height_scale = 2.0f;

	float jumpTimeCnt;
	constexpr float jump_time = 0.35f;
	constexpr float jump_velocity = 300.0f;
	constexpr float remain_jump_velocity = 300.0f;
	constexpr float side_move_velocity = 200.0f;

	constexpr unsigned int idle_animation_speed = 100;
	constexpr unsigned int run_animation_speed = 100;
	constexpr unsigned int jump_animation_speed = 100;
	constexpr unsigned int fall_animation_speed = 100;

	bool isJumping = false;
}

Player::Player(GameScene& gs):gs_(gs)
{
	rigidBody_ = nullptr;
}

void Player::Initialize()
{
	input_ = std::make_unique<KeyboardInput>();

	self_ = gs_.entityMng_->AddEntity("player");
	self_->AddComponent<TransformComponent>(start_pos, player_width, player_height, player_scale);
	auto& rigidBody = gs_.collisionMng_->AddRigidBody2D(
		self_, 
		start_pos,
		player_width * rigidbody_width_scale,
		player_height * rigidbody_height_scale);
	rigidBody_ = &rigidBody;
	rigidBody.tag_ = "PLAYER";
	self_->AddComponent<SpriteComponent>();
	auto playerAnim = self_->GetComponent<SpriteComponent>();
	playerAnim->AddAnimation(gs_.assetMng_->GetTexture("player-idle"), "idle", Rect(0, 0, 32, 32), idle_animation_speed, 4);
	playerAnim->AddAnimation(gs_.assetMng_->GetTexture("player-run"), "run", Rect(0, 0, 32, 32), run_animation_speed, 6);
	playerAnim->AddAnimation(gs_.assetMng_->GetTexture("player-jump"), "jump", Rect(0, 0, 32, 32), jump_animation_speed, 4);
	playerAnim->AddAnimation(gs_.assetMng_->GetTexture("player-fall"), "fall", Rect(0, 0, 32, 32), fall_animation_speed, 2);
	playerAnim->Play("idle");

	// Set camera track to player
	Camera::Instance().TrackingOn(self_->GetComponent<TransformComponent>());

	// Initialize Equipment list
	equipments_.emplace_back(std::move(std::make_unique<ShurikenEquip>(gs_)));
	equipments_.emplace_back(std::move(std::make_unique<BombEquip>(gs_)));
	
}

void Player::Input(const float& deltaTime)
{
	auto sprite = self_->GetComponent<SpriteComponent>();
	input_->Update(deltaTime);
	if (input_->IsPressed(L"left"))
	{
		rigidBody_->velocity_.X = -side_move_velocity;
		sprite->isFlipped = true;
		
	}
	else if (input_->IsPressed(L"right"))
	{
		rigidBody_->velocity_.X = side_move_velocity;
		sprite->isFlipped = false;
	}
	else
	{
		rigidBody_->velocity_.X = 0;
	}

	// Jump
	if (input_->IsTriggered(L"up") && rigidBody_->isGrounded_)
	{
		rigidBody_->velocity_.Y = -jump_velocity;
		jumpTimeCnt = jump_time;
		isJumping = true;
	}

	// Remain jump
	if (input_->IsPressed(L"up") && isJumping)
	{
		if (jumpTimeCnt > 0)
		{
			rigidBody_->velocity_.Y = -remain_jump_velocity;
			jumpTimeCnt -= deltaTime;
		}
		else
		{
			isJumping = false;
		}

	}
	if (input_->IsReleased(L"up"))
	{
		isJumping = false;
	}

	if (input_->IsTriggered(L"switch"))
	{
		currentEquip_ = (currentEquip_ + 1) % equipments_.size();
	}

	SetAngleDirection();
	if (input_->IsTriggered(L"attack"))
	{
		auto transform = self_->GetComponent<TransformComponent>();
		auto startPos = transform->pos + Vector2(transform->w / 2, transform->h / 2);
		equipments_[currentEquip_]->Attack(startPos, attackAngle_);
	}

}

void Player::SetAngleDirection()
{
	if (input_->IsPressed(L"left"))
	{
		attackAngle_ = atan2f(0, -1);
		if (input_->IsPressed(L"up"))
		{
			attackAngle_ = atan2f(-1, -1);
		}
	}
	else if (input_->IsPressed(L"right"))
	{
		attackAngle_ = atan2f(0, 1);
		if (input_->IsPressed(L"up"))
		{
			attackAngle_ = atan2f(-1, 1);
		}
	}
	else if (input_->IsPressed(L"up"))
	{
		attackAngle_ = atan2f(-1, 0);
	}
	else if (input_->IsPressed(L"down"))
	{
		attackAngle_ = atan2f(1, 0);
	}
}

void Player::UpdateState()
{
	auto sprite = self_->GetComponent<SpriteComponent>();

	if (rigidBody_->isGrounded_)
	{
		if (rigidBody_->velocity_.X > 0)
		{
			moveState_ = MOVE::LEFT;
		}
		else if (rigidBody_->velocity_.X < 0)
		{
			moveState_ = MOVE::RIGHT;
		}
	}
	
	if (!rigidBody_->isGrounded_ && isJumping)
	{
		moveState_ = MOVE::JUMP;
	}
	if (!rigidBody_->isGrounded_ && (rigidBody_->velocity_.Y > 0))
	{
		moveState_ = MOVE::FALL;
	}

	if (rigidBody_->isGrounded_ && (rigidBody_->velocity_.X == 0)) moveState_ = MOVE::IDLE;

	switch (moveState_)
	{
	case MOVE::LEFT:
		sprite->Play("run");
		break;
	case MOVE::RIGHT:
		sprite->Play("run");
		break;
	case MOVE::JUMP:
		sprite->Play("jump");
		break;
	case MOVE::FALL:
		sprite->Play("fall");
		break;
	case MOVE::IDLE:
		sprite->Play("idle");
		break;
	}
}

std::shared_ptr<TransformComponent> Player::GetPlayerTransform()
{
	return self_->GetComponent<TransformComponent>();
}

void Player::RenderUI()
{
	equipments_[currentEquip_]->Render();
}

Player::~Player()
{
}
