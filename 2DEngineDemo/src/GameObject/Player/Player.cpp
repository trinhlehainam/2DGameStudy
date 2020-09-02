#include "Player.h"

#include "../Entity.h"
#include "../../Geometry/Geometry.h"
#include "../../Input/KeyboardInput.h"
#include "../Player/ShurikenEquip.h"
#include "../Player/BombEquip.h"
#include "../Player/SwordEquip.h"

#include "../../Scene/GameScene.h"
#include "../../System/AssetManager.h"
#include "../../System/Camera.h"
#include "../../System/CollisionManager.h"
#include "../../System/EntityManager.h"

#include "../../Component/TransformComponent.h"
#include "../../Component/Collider/RigidBody2D.h"
#include "../../Component/SpriteComponent.h"

namespace
{
	// Initialize
	const Rect src_rect = Rect(0, 0, 32, 32);
	const Vector2 start_pos = Vector2(30.0f, 512.0f);
	constexpr float player_width = 32.0f;
	constexpr float player_height = 32.0f;
	constexpr float player_scale = 2.0f;

	// Melee Attack
	const Rect attack1_src_rect = Rect(0, 0, 42, 36);
	const Rect attack2_src_rect = Rect(0, 0, 45, 29);
	const Rect attack3_src_rect = Rect(0, 0, 50, 26);
	const Vector2 attack1_offset = Vector2(0, (attack1_src_rect.h - player_height) * player_scale);
	const Vector2 attack2_offset = Vector2(10, (attack2_src_rect.h - player_height) * player_scale);
	const Vector2 attack3_offset = Vector2(10, (attack3_src_rect.h - player_height) * player_scale);

	// Animation's hit box
	constexpr float rigidbody_width_scale = 1.2f;
	constexpr float rigidbody_height_scale = 2.0f;
	constexpr float rigidbody_jump_scale = 1.5f;
	constexpr float rigidbody_crouch_scale = 1.4f;

	// Time counter ( millisecond )
	constexpr float jump_time = 0.35f;
	constexpr float change_attack_time = 0.07f;
	constexpr float cooldown_attack_time = 0.2f;

	// Movement's velocity
	constexpr float jump_velocity = 300.0f;
	constexpr float remain_jump_velocity = 300.0f;
	constexpr float normal_side_velocity = 200.0f;
	constexpr float crouch_velocity = 50.0f;

	// Animation's speed
	constexpr unsigned short int idle_animation_speed = 100;
	constexpr unsigned short int run_animation_speed = 100;
	constexpr unsigned short int fast_run_animation_speed = 50;
	constexpr unsigned short int jump_animation_speed = 100;
	constexpr unsigned short int fall_animation_speed = 100;
	constexpr unsigned short int hurt_animation_speed = 100;
	constexpr unsigned short int die_animation_speed = 100;
	constexpr unsigned short int cast_animation_speed = 50;
	constexpr unsigned short int crouch_animation_speed = 100;
	constexpr unsigned short int attack1_animation_speed = 70;
	constexpr unsigned short int attack2_animation_speed = 70;
	constexpr unsigned short int attack3_animation_speed = 70;
	constexpr unsigned short int draw_withdraw_animation_speed = 100;

	// Attack frame
	// Attack collider is created at this frame
	constexpr unsigned short int attack1_frame = 3;
	constexpr unsigned short int attack2_frame = 4;
	constexpr unsigned short int attack3_frame = 3;

	// Weapon's damage
	constexpr int shuriken_damage = 1;
	constexpr int bomb_damage = 5;
	constexpr int attack1_damage = 1;

	constexpr char sword_tag[] = "SWORD";
	constexpr char shuriken_tag[] = "BOMB";
	constexpr char bomb_tag[] = "SHURIKEN";
}

Player::Player(GameScene& gs):gs_(gs)
{
	rigidBody_ = nullptr;
}

void Player::Initialize()
{
	input_ = std::make_unique<KeyboardInput>();
	inputState_ = &Player::GroundState;
	oldInputState_ = inputState_;

	self_ = gs_.entityMng_->AddEntity("player");
	self_->AddComponent<TransformComponent>(self_, start_pos, player_width, player_height, player_scale);
	const auto& rigidBody = gs_.collisionMng_->AddRigidBody2D(
		self_, 
		start_pos,
		player_width * rigidbody_width_scale,
		player_height * rigidbody_height_scale);
	rigidBody_ = rigidBody;
	rigidBody->SetTag("PLAYER");
	self_->AddComponent<SpriteComponent>(self_);
	const auto& playerAnim = self_->GetComponent<SpriteComponent>();
	playerAnim->AddAnimation(gs_.assetMng_->GetTexture("player-idle"), "idle", src_rect, idle_animation_speed);
	playerAnim->AddAnimation(gs_.assetMng_->GetTexture("player-sword-idle"), "sword-idle", src_rect, idle_animation_speed);
	playerAnim->AddAnimation(gs_.assetMng_->GetTexture("player-run"), "run", src_rect, run_animation_speed);
	playerAnim->AddAnimation(gs_.assetMng_->GetTexture("player-sword-run"), "sword-run", src_rect, run_animation_speed);
	playerAnim->AddAnimation(gs_.assetMng_->GetTexture("player-fast-run"), "fast-run", src_rect, fast_run_animation_speed);
	playerAnim->AddAnimation(gs_.assetMng_->GetTexture("player-jump"), "jump", src_rect, jump_animation_speed);
	playerAnim->AddAnimation(gs_.assetMng_->GetTexture("player-fall"), "fall", src_rect, fall_animation_speed);
	playerAnim->AddAnimation(gs_.assetMng_->GetTexture("player-hurt"), "hurt", src_rect, hurt_animation_speed);
	playerAnim->AddAnimation(gs_.assetMng_->GetTexture("player-die"), "die", src_rect, die_animation_speed);
	playerAnim->AddAnimation(gs_.assetMng_->GetTexture("player-crouch"), "crouch", src_rect, crouch_animation_speed);
	playerAnim->AddAnimation(gs_.assetMng_->GetTexture("player-crouch-walk"), "crouch-walk", src_rect, crouch_animation_speed);
	playerAnim->AddAnimation(gs_.assetMng_->GetTexture("player-cast"), "cast", src_rect, cast_animation_speed);
	playerAnim->AddAnimation(gs_.assetMng_->GetTexture("player-draw-sword"), "draw", src_rect, draw_withdraw_animation_speed);
	playerAnim->AddAnimation(gs_.assetMng_->GetTexture("player-withdraw-sword"), "withdraw", src_rect, draw_withdraw_animation_speed);
	playerAnim->AddAnimation(gs_.assetMng_->GetTexture("player-attack1"), "attack-1", attack1_src_rect, attack1_animation_speed);
	playerAnim->AddAnimation(gs_.assetMng_->GetTexture("player-attack2"), "attack-2", attack2_src_rect, attack2_animation_speed);
	playerAnim->AddAnimation(gs_.assetMng_->GetTexture("player-attack3"), "attack-3", attack3_src_rect, attack3_animation_speed);
	playerAnim->PlayLoop("idle");

	// Set animation offset
	playerAnim->SetAnimationOffset("attack-1", attack1_offset);
	playerAnim->SetAnimationOffset("attack-2", attack2_offset);
	playerAnim->SetAnimationOffset("attack-3", attack3_offset);

	// Set camera track to player
	Camera::Instance().TrackingOn(self_->GetComponent<TransformComponent>());

	// Initialize Equipment list
	equipments_.emplace_back(std::move(std::make_unique<ShurikenEquip>(gs_, shuriken_tag, self_, shuriken_damage)));
	equipments_.emplace_back(std::move(std::make_unique<BombEquip>(gs_, bomb_tag, self_,bomb_damage)));
	equipments_.emplace_back(std::move(std::make_unique<SwordEquip>(gs_, sword_tag, self_)));
}

void Player::Input(const float& deltaTime)
{
	input_->Update(deltaTime);
	SetAngleDirection();
	ChangeEquip();
	const auto& sprite = self_->GetComponent<SpriteComponent>();
	(this->*inputState_)(deltaTime);
}

void Player::GroundState(const float& deltaTime)
{
	SetSideMoveVelocity(normal_side_velocity);
	SetMoveAction(ACTION::IDLE,ACTION::NORMAL_RUN);

	ProcessFall();
	ProcessJump();
	ProcessThrow();
	ProcessDrawWithdrawSword();
	ProcessGroundAttack();
	if (input_->IsPressed(L"down"))
	{
		isCrouch = true;
		actionState_ = ACTION::CROUCH;
		inputState_ = &Player::CrouchState;
	}	
}

void Player::ProcessThrow()
{
	if (equipments_[currentEquip_]->GetTag() == sword_tag) return;
	if (input_->IsTriggered(L"attack") && !isDrawn)
	{
		const auto& transform = self_->GetComponent<TransformComponent>();
		auto startPos = transform->pos + Vector2(transform->w / 2, transform->h / 2);
		equipments_[currentEquip_]->Attack(startPos, attackAngle_);

		if (actionState_ == ACTION::JUMP)
		{
			oldActionState_= ACTION::FALL;
			oldInputState_ = &Player::FallState;
			isJumping = false;
		}
		else
		{
			oldActionState_ = actionState_;
			oldInputState_ = inputState_;
		}
		inputState_ = &Player::ThrowState;
		actionState_ = ACTION::THROW;
	}
}

void Player::ProcessGroundAttack()
{
	if (equipments_[currentEquip_]->GetTag() != sword_tag) return;
	if (input_->IsPressed(L"attack") && isDrawn)
	{
		switch (actionState_)
		{
		case ACTION::ATTACK_1:
			timer_ = change_attack_time;
			actionState_ = ACTION::ATTACK_2;
			break;
		case ACTION::ATTACK_2:
			timer_ = change_attack_time;
			actionState_ = ACTION::ATTACK_3;
			break;
		case ACTION::ATTACK_3:
			timer_ = change_attack_time;
			actionState_ = ACTION::ATTACK_1;
		default:
			actionState_ = ACTION::ATTACK_1;
			timer_ = change_attack_time;
			break;
		}
		inputState_ = &Player::GroundAttackState;
		isMeleeActive = true;
	}
}

void Player::ThrowState(const float&)
{
	const auto& sprite = self_->GetComponent<SpriteComponent>();

	if (rigidBody_->isGrounded_)
		rigidBody_->velocity_.X = 0.0f;

	if (sprite->IsFinished())
	{
		TurnBackState();
	}
}

void Player::ProcessJump()
{
	if (input_->IsTriggered(L"jump") && rigidBody_->isGrounded_)
	{
		rigidBody_->velocity_.Y = -jump_velocity;
		rigidBody_->isGrounded_ = false;
		timer_ = jump_time;
		isJumping = true;
		actionState_ = ACTION::JUMP;
		inputState_ = &Player::JumpState;
	}
	if (isJumping)
	{
		actionState_ = ACTION::JUMP;
		inputState_ = &Player::JumpState;
	}
}

void Player::JumpState(const float& deltaTime)
{
	SetSideMoveVelocity(normal_side_velocity);
	ProcessThrow();
	if (input_->IsPressed(L"jump") && isJumping)
	{
		if (timer_ > 0)
		{
			rigidBody_->velocity_.Y = -remain_jump_velocity;
			timer_ -= deltaTime;
		}
		else
		{
			isJumping = false;
		}
	}

	if (input_->IsReleased(L"jump"))
	{
		isJumping = false;
	}

	if (!isJumping)
	{
		rigidBody_->isGrounded_ = false;
		actionState_ = ACTION::FALL;
		inputState_ = &Player::FallState;
	}
}

void Player::FallState(const float& deltaTime)
{
	SetSideMoveVelocity(normal_side_velocity);
	ProcessThrow();
	ProcessCheckGround();
	// Second jump
	if (input_->IsTriggered(L"jump"))
	{
		rigidBody_->velocity_.Y = -jump_velocity;
		isJumping = true;
		actionState_ = ACTION::JUMP;
		inputState_ = &Player::SecondJumpState;
	}
}

void Player::CrouchState(const float&)
{
	SetSideMoveVelocity(crouch_velocity);
	SetMoveAction(ACTION::CROUCH, ACTION::CROUCH_WALK);
	if (input_->IsReleased(L"down"))
	{
		isCrouch = false;
		inputState_ = &Player::GroundState;
	}
}

void Player::SecondJumpState(const float& deltaTime)
{
	const auto& sprite = self_->GetComponent<SpriteComponent>();

	SetSideMoveVelocity(normal_side_velocity);
	ProcessThrow();
	if (isJumping && sprite->IsFinished())
	{
		isJumping = false;
		actionState_ = ACTION::FALL;
	}
	ProcessCheckGround();
}

void Player::GroundAttackState(const float& deltaTime)
{
	const auto& sprite = self_->GetComponent<SpriteComponent>();
	const auto& transform = self_->GetComponent<TransformComponent>();
	rigidBody_->velocity_.X = 0;
	
	if (isMeleeActive)
	{
		switch (actionState_)
		{
		case ACTION::ATTACK_1:
			
			SetMeleeAttack(1, attack1_frame, attackAngle_, transform->pos, transform->h, transform->w);
			break;
		case ACTION::ATTACK_2:
			equipments_[currentEquip_]->SetDamage(1);
			SetMeleeAttack(2, attack2_frame, attackAngle_, transform->pos, transform->h, transform->w);
			break;
		case ACTION::ATTACK_3:
			equipments_[currentEquip_]->SetDamage(1);
			SetMeleeAttack(2, attack3_frame, attackAngle_, transform->pos, transform->h, transform->w);
			break;
		}
	}
	
	if (sprite->IsFinished())
	{
		if (timer_ > 0)
		{
			timer_ -= deltaTime;
			ProcessGroundAttack();
		}
		else
		{
			inputState_ = &Player::GroundState;
			timer_ = 0.0f;
		}
	}

}

void Player::DrawWithdrawSwordState(const float&)
{
	if (rigidBody_->isGrounded_)
		rigidBody_->velocity_.X = 0.0f;
	const auto& sprite = self_->GetComponent<SpriteComponent>();
	if (sprite->IsFinished())
		TurnBackState();
}

void Player::SetSideMoveVelocity(const float& velX)
{
	const auto& sprite = self_->GetComponent<SpriteComponent>();
	if (input_->IsPressed(L"left"))
	{
		rigidBody_->velocity_.X = -velX;
		sprite->isFlipped = true;
	}
	else if (input_->IsPressed(L"right"))
	{
		rigidBody_->velocity_.X = velX;
		sprite->isFlipped = false;
	}
	else
	{
		rigidBody_->velocity_.X = 0;
	}
}

void Player::SetMoveAction(const ACTION& idle, const ACTION& moveType)
{
	if (rigidBody_->velocity_.X != 0.0f)
		actionState_ = moveType;
	else
		actionState_ = idle;
}

void Player::SetMeleeAttack(const int& damage, const unsigned int& frame_no, const float& dirAngle, const Vector2& pos,
	const float& w, const float& h)
{
	const auto& sprite = self_->GetComponent<SpriteComponent>();
	equipments_[currentEquip_]->SetDamage(damage);
	if (sprite->GetCurrentCelNO() == frame_no)
	{
		equipments_[currentEquip_]->Attack(pos, dirAngle, w, h);
		isMeleeActive = false;
	}
}

void Player::ProcessCheckGround()
{
	if (rigidBody_->isGrounded_)
	{
		inputState_ = &Player::GroundState;
		actionState_ = ACTION::IDLE;
	}
}

void Player::ProcessFall()
{
	if (rigidBody_->velocity_.Y > 0.0f && !isJumping)
	{
		rigidBody_->isGrounded_ = false;
		actionState_ = ACTION::FALL;
		inputState_ = &Player::FallState;
	}
}

void Player::ProcessDrawWithdrawSword()
{
	if (equipments_[currentEquip_]->GetTag() == sword_tag)
	{
		if (!isDrawn)
		{
			actionState_ = ACTION::DRAW_SWORD;
			oldInputState_ = inputState_;
			inputState_ = &Player::DrawWithdrawSwordState;
			isDrawn = true;
		}
	}
	else
	{
		if (isDrawn)
		{
			actionState_ = ACTION::WITHDRAW_SWORD;
			oldInputState_ = inputState_;
			inputState_ = &Player::DrawWithdrawSwordState;
			isDrawn = false;
		}
	}
}

void Player::ChangeEquip()
{
	if (input_->IsTriggered(L"switch"))
	{
		currentEquip_ = (currentEquip_ + 1) % equipments_.size();
	}
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

void Player::TurnBackState()
{
	inputState_ = oldInputState_;
	actionState_ = oldActionState_;
}

void Player::UpdateState()
{
	const auto& sprite = self_->GetComponent<SpriteComponent>();
	const auto& transform = self_->GetComponent<TransformComponent>();

	rigidBody_->collider_.h = player_height * rigidbody_height_scale;

	switch (actionState_)
	{
	case ACTION::NORMAL_RUN:
		if (isDrawn)
			sprite->PlayLoop("sword-run");
		else
			sprite->PlayLoop("run");
		break;
	case ACTION::JUMP:
		sprite->PlayLoop("jump");
		rigidBody_->collider_.h = player_height * rigidbody_jump_scale;
		break;
	case ACTION::FALL:
		sprite->PlayLoop("fall");
		break;
	case ACTION::IDLE:
		if (isDrawn)
			sprite->PlayLoop("sword-idle");
		else
			sprite->PlayLoop("idle");
		break;
	case ACTION::THROW:
		sprite->PlayOnce("cast");
		break;
	case ACTION::CROUCH:
		rigidBody_->collider_.h = player_height * rigidbody_crouch_scale;
		sprite->PlayLoop("crouch");
		break;
	case ACTION::CROUCH_WALK:
		rigidBody_->collider_.h = player_height * rigidbody_crouch_scale;
		sprite->PlayLoop("crouch-walk");
		break;
	case ACTION::ATTACK_1:
		sprite->PlayOnce("attack-1");
		break;
	case ACTION::ATTACK_2:
		sprite->PlayOnce("attack-2");
		break;
	case ACTION::ATTACK_3:
		sprite->PlayOnce("attack-3");
		break;
	case ACTION::DRAW_SWORD:
		sprite->PlayOnce("draw");
		break;
	case ACTION::WITHDRAW_SWORD:
		sprite->PlayOnce("withdraw");
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
