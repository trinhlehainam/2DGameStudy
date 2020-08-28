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
	const Rect src_rect = Rect(0, 0, 32, 32);
	const Vector2 start_pos = Vector2(30.0f, 500.0f);
	constexpr float player_width = 32.0f;
	constexpr float player_height = 32.0f;
	constexpr float player_scale = 2.0f;

	constexpr float rigidbody_width_scale = 1.2f;
	constexpr float rigidbody_height_scale = 2.0f;
	constexpr float rigidbody_jump_scale = 1.5f;
	constexpr float rigidbody_crouch_scale = 1.4f;

	float jumpTimeCnt;
	constexpr float jump_time = 0.35f;
	constexpr float jump_velocity = 300.0f;
	constexpr float remain_jump_velocity = 300.0f;
	constexpr float side_move_velocity = 200.0f;

	constexpr unsigned int idle_animation_speed = 100;
	constexpr unsigned int run_animation_speed = 100;
	constexpr unsigned int jump_animation_speed = 100;
	constexpr unsigned int fall_animation_speed = 100;
	constexpr unsigned int hurt_animation_speed = 100;
	constexpr unsigned int die_animation_speed = 100;
	constexpr unsigned int cast_animation_speed = 50;
	constexpr unsigned int crouch_animation_speed = 100;

	bool isJumping = false;
}

Player::Player(GameScene& gs):gs_(gs)
{
	rigidBody_ = nullptr;
}

void Player::Initialize()
{
	input_ = std::make_unique<KeyboardInput>();
	processInput_ = &Player::GroundInput;
	oldInputState_ = processInput_;

	self_ = gs_.entityMng_->AddEntity("player");
	self_->AddComponent<TransformComponent>(start_pos, player_width, player_height, player_scale);
	auto rigidBody = gs_.collisionMng_->AddRigidBody2D(
		self_, 
		start_pos,
		player_width * rigidbody_width_scale,
		player_height * rigidbody_height_scale);
	rigidBody_ = rigidBody;
	rigidBody->tag_ = "PLAYER";
	self_->AddComponent<SpriteComponent>();
	auto playerAnim = self_->GetComponent<SpriteComponent>();
	playerAnim->AddAnimation(gs_.assetMng_->GetTexture("player-idle"), "idle", src_rect, idle_animation_speed);
	playerAnim->AddAnimation(gs_.assetMng_->GetTexture("player-run"), "run", src_rect, run_animation_speed);
	playerAnim->AddAnimation(gs_.assetMng_->GetTexture("player-jump"), "jump", src_rect, jump_animation_speed);
	playerAnim->AddAnimation(gs_.assetMng_->GetTexture("player-fall"), "fall", src_rect, fall_animation_speed);
	playerAnim->AddAnimation(gs_.assetMng_->GetTexture("player-hurt"), "hurt", src_rect, hurt_animation_speed);
	playerAnim->AddAnimation(gs_.assetMng_->GetTexture("player-die"), "die", src_rect, die_animation_speed);
	playerAnim->AddAnimation(gs_.assetMng_->GetTexture("player-crouch"), "crouch", src_rect, crouch_animation_speed);
	playerAnim->AddAnimation(gs_.assetMng_->GetTexture("player-cast"), "cast", src_rect, cast_animation_speed);
	playerAnim->Play("idle");

	// Set camera track to player
	Camera::Instance().TrackingOn(self_->GetComponent<TransformComponent>());

	// Initialize Equipment list
	equipments_.emplace_back(std::move(std::make_unique<ShurikenEquip>(gs_)));
	equipments_.emplace_back(std::move(std::make_unique<BombEquip>(gs_)));
	
}

void Player::Input(const float& deltaTime)
{
	Move(deltaTime);
	SetAngleDirection();
	ChangeEquip(deltaTime);
	(this->*processInput_)(deltaTime);
}

void Player::Move(const float& deltaTime)
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
}

void Player::GroundInput(const float& deltaTime)
{
	if (rigidBody_->isGrounded_)
	{
		if (rigidBody_->velocity_.X > 0)
		{
			actionState_ = ACTION::LEFT;
		}
		else if (rigidBody_->velocity_.X < 0)
		{
			actionState_ = ACTION::RIGHT;
		}
		else
		{
			actionState_ = ACTION::IDLE;
		}
	}
	if (!rigidBody_->isGrounded_ && !isJumping)
	{
		processInput_ = &Player::FallInput;
		actionState_ = ACTION::FALL;
	}

	JumpInput(deltaTime);
	Attack(deltaTime);
	if (input_->IsPressed(L"down"))
	{
		actionState_ = ACTION::CROUCH;
		processInput_ = &Player::CrouchInput;
	}	
}

void Player::JumpInput(const float& deltaTime)
{
	if (input_->IsTriggered(L"jump") && rigidBody_->isGrounded_)
	{
		rigidBody_->velocity_.Y = -jump_velocity;
		rigidBody_->isGrounded_ = false;
		jumpTimeCnt = jump_time;
		isJumping = true;
		actionState_ = ACTION::JUMP;
		processInput_ = &Player::RemainJump;
	}
}

void Player::RemainJump(const float& deltaTime)
{
	Attack(deltaTime);
	if (input_->IsPressed(L"jump") && isJumping)
	{
		if (jumpTimeCnt > 0)
		{
			rigidBody_->velocity_.Y = -remain_jump_velocity;
			jumpTimeCnt -= deltaTime;
		}
		else
		{
			isJumping = false;
			actionState_ = ACTION::FALL;
			processInput_ = &Player::FallInput;
		}

	}
	if (input_->IsReleased(L"jump"))
	{
		isJumping = false;
		actionState_ = ACTION::FALL;
		processInput_ = &Player::FallInput;
	}

}

void Player::FallInput(const float& deltaTime)
{
	Attack(deltaTime);
	ProcessCheckGround();
	// Second jump
	if (input_->IsTriggered(L"jump"))
	{
		rigidBody_->velocity_.Y = -jump_velocity;
		isJumping = true;
		actionState_ = ACTION::JUMP;
		processInput_ = &Player::SecondJumpInput;
	}
}

void Player::ProcessCheckGround()
{
	if (rigidBody_->isGrounded_)
	{
		processInput_ = &Player::GroundInput;
		actionState_ = ACTION::IDLE;
	}
}

void Player::ChangeEquip(const float& deltaTime)
{
	if (input_->IsTriggered(L"switch"))
	{
		currentEquip_ = (currentEquip_ + 1) % equipments_.size();
	}
}

void Player::Attack(const float& deltaTime)
{
	if (input_->IsTriggered(L"throw"))
	{
		auto transform = self_->GetComponent<TransformComponent>();
		auto startPos = transform->pos + Vector2(transform->w / 2, transform->h / 2);
		equipments_[currentEquip_]->Attack(startPos, attackAngle_);

		oldState_ = actionState_;
		actionState_ = ACTION::THROW;
		oldInputState_ = processInput_;
		processInput_ = &Player::Throw;
	}
}

void Player::Throw(const float&)
{
	auto sprite = self_->GetComponent<SpriteComponent>();

	if(rigidBody_->isGrounded_)
		rigidBody_->velocity_.X = 0.0f;

	if (sprite->IsFinished())
	{
		processInput_ = oldInputState_;
		actionState_ = oldState_;
	}
}

void Player::CrouchInput(const float&)
{
	rigidBody_->velocity_.X = 0.0f;
	rigidBody_->velocity_.Y = 1.0f;
	if (input_->IsReleased(L"down"))
		processInput_ = &Player::GroundInput;
}

void Player::SecondJumpInput(const float& deltaTime)
{
	auto sprite = self_->GetComponent<SpriteComponent>();

	Attack(deltaTime);
	if (isJumping && sprite->IsFinished())
	{
		isJumping = false;
		actionState_ = ACTION::FALL;
	}
	ProcessCheckGround();
}

void Player::SetAngleDirection()
{
	if (input_->IsPressed(L"left"))
	{
		attackAngle_ = atan2f(0, -1);
	}
	else if (input_->IsPressed(L"right"))
	{
		attackAngle_ = atan2f(0, 1);
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
	auto transform = self_->GetComponent<TransformComponent>();

	rigidBody_->collider_.h = player_height * rigidbody_height_scale;

	switch (actionState_)
	{
	case ACTION::LEFT:
		sprite->Play("run");
		break;
	case ACTION::RIGHT:
		sprite->Play("run");
		break;
	case ACTION::JUMP:
		sprite->Play("jump");
		rigidBody_->collider_.h = player_height * rigidbody_jump_scale;
		break;
	case ACTION::FALL:
		sprite->Play("fall");
		break;
	case ACTION::IDLE:
		sprite->Play("idle");
		break;
	case ACTION::THROW:
		sprite->Play("cast");
		break;
	case ACTION::CROUCH:
		rigidBody_->collider_.h = player_height * rigidbody_crouch_scale;
		sprite->Play("crouch");
		break;
	}

	// Relocate rigid body's position after change it's size
	rigidBody_->collider_.pos.X = transform->pos.X + transform->w * transform->scale / 2.0f - rigidBody_->collider_.w / 2;
	rigidBody_->collider_.pos.Y = transform->pos.Y + transform->h * transform->scale - rigidBody_->collider_.h;
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
