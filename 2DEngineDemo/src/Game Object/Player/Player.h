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
	CROUCH_WALK
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
	ACTION oldState_ = actionState_;
	using Input_t = void (Player::*)(const float&);
	Input_t processInput_;
	Input_t oldInputState_;
	void GroundInput(const float&);
	void JumpInput(const float&);
	void RemainJump(const float&);
	void FallInput(const float&);
	void ChangeEquip(const float&);
	void Attack(const float&);
	void Throw(const float&);
	void CrouchInput(const float&);
	void SecondJumpInput(const float&);

	void SideMove(const float& velX);
	void SetMoveAction(const ACTION& idle, const ACTION& moveType);
	void ProcessCheckGround();
	void ProcessFall();
	void SetAngleDirection();
public:
	void UpdateState();
	Player(GameScene& gs);
	void Initialize();
	void Input(const float& deltaTime);
	std::shared_ptr<TransformComponent> GetPlayerTransform();
	void RenderUI();
	~Player();
};

