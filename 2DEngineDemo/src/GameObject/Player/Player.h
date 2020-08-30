#pragma once

#include <memory>
#include <vector>

class GameScene;
class RigidBody2D;
class KeyboardInput;
class Entity;
class TransformComponent;
class Equipment;

enum class ACTION
{
	IDLE,
	NORMAL_RUN,
	SWORD_RUN,
	JUMP,
	FALL,
	THROW,
	CROUCH,
	CROUCH_WALK,
	ATTACK_1,
	ATTACK_2,
	ATTACK_3,
	DRAW_SWORD,
	WITHDRAW_SWORD
};

class Player
{
private:
	friend GameScene;
	GameScene& gs_;
	std::shared_ptr<RigidBody2D> rigidBody_;
	std::unique_ptr<KeyboardInput> input_;
	std::shared_ptr<Entity> self_;

	std::vector<std::unique_ptr<Equipment>> equipments_;
	int currentEquip_ = 0;
	float attackAngle_ = 0.0f;
	bool isDrawn = false;
	bool isJumping = false;
	bool isCrouch = false;

	ACTION actionState_ = ACTION::IDLE;
	ACTION oldActionState_ = actionState_;
	using Input_t = void (Player::*)(const float&);
	Input_t inputState_;
	Input_t oldInputState_;
	void GroundState(const float&);
	void JumpState(const float&);
	void FallState(const float&);
	void ChangeEquip(const float&);
	void ThrowState(const float&);
	void CrouchState(const float&);
	void SecondJumpState(const float&);
	void DrawWithdrawSwordState(const float&);
	void MeleeAttack(const float&);
	
	void SetSideMoveVelocity(const float& velX);
	void SetMoveAction(const ACTION& idle, const ACTION& moveType);
	void SetAngleDirection();
	void ProcessAttack();
	void ProcessJump();
	void ProcessCheckGround();
	void ProcessFall();
	void ProcessDrawWithdrawSword();
	void TurnBackState();
	
public:
	void UpdateState();
	Player(GameScene& gs);
	void Initialize();
	void Input(const float& deltaTime);
	std::shared_ptr<TransformComponent> GetPlayerTransform();
	void RenderUI();
	~Player();
};

