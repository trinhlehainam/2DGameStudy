#pragma once

#include <memory>
#include <vector>

class GameScene;
class RigidBody2D;
class KeyboardInput;
class Entity;
class TransformComponent;
class Equipment;
class Vector2;
class Rect;

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
	AIR_ATTACK,
	AIR_CHARGE,
	SLASH_DOWN,
	SMASH_DOWN,
	DRAW_SWORD,
	WITHDRAW_SWORD,
	SLIDE_WALL,
	HURT,
	DEATH
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
	int ammountOfJump_ = 2;
	int jumpCnt_ = 0;
	float timer_;
	bool isAlive_ = true;
	bool isDrawn = false;
	bool isJumping = false;
	bool isCrouch = false;
	bool isMeleeActive = false;
	bool isSlidingWall = false;
	bool isWallJumpActive = false;
	bool isAirAttackActive = false;

	ACTION actionState_ = ACTION::IDLE;
	ACTION oldActionState_ = actionState_;
	using Input_t = void (Player::*)(const float&);
	Input_t inputState_;
	Input_t oldInputState_;
	void GroundState(const float&);
	void JumpState(const float&);
	void FallState(const float&);
	void ThrowState(const float&);
	void CrouchState(const float&);
	void DrawWithdrawSwordState(const float&);
	void GroundAttackState(const float&);
	void AirAttackState(const float&);
	void AirChargeState(const float&);
	void SlashDownState(const float&);
	void SmashDownState(const float&);
	void SlidingWallState(const float&);
	void HurtState(const float&);
	void DeathState(const float&);
	void CheckHit();

	void SetSideMoveVelocity(const float& velX);
	void SetMoveAction(const ACTION& idle, const ACTION& moveType);
	void SetMeleeAttack(const int& damage, const unsigned int& frame_no, bool flipFlag,
		const Vector2& offset, const Rect& srcRect);
	void SetAngleDirection();
	void ProcessThrow();
	void ProcessGroundAttack();
	void ProcessAirAttack();
	void ProcessJump();
	void ProcessCheckGround();
	void ProcessFall();
	void ProcessDrawWithdrawSword();
	void ProcessSlidingWall();
	void ChangeEquip();
	void TurnBackState();

public:
	void UpdateState();
	inline bool IsAlive() const { return isAlive_; }
	inline void SetAlive() { isAlive_ = true; }
	void SetPosition(const float& posX, const float& posY);
	float Width() const;
	float Height() const;
	void ForceFall();
	Player(GameScene& gs);
	void Initialize();
	void Input(const float& deltaTime);
	std::shared_ptr<TransformComponent> GetPlayerTransform();
	void RenderUI();
	~Player();
};

