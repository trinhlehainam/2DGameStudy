#pragma once

#include <memory>
#include <vector>

class GameScene;
class RigidBody2D;
class KeyboardInput;
class Entity;
class TransformComponent;
class Equipment;

enum class MOVE
{
	IDLE,
	LEFT,
	RIGHT,
	JUMP,
	FALL,
	THROW
};

class Player
{
private:
	friend GameScene;
	GameScene& gs_;
	std::shared_ptr<RigidBody2D> rigidBody_;
	std::unique_ptr<KeyboardInput> input_;
	std::shared_ptr<Entity> self_;
	MOVE moveState_ = MOVE::IDLE;

	using Input_t = void (Player::*)(const float&);
	Input_t processInput_;
	void Default(const float&);
	void Jump(const float&);
	void RemainJump(const float&);
	void Fall(const float&);
	void ChangeEquip(const float&);
	void Attack(const float&);

	std::vector<std::unique_ptr<Equipment>> equipments_;
	int currentEquip_ = 0;
	float attackAngle_ = 0.0f;
	
	void SetAngleDirection();
public:
	void UpdateState();
	Player(GameScene& gs);
	void Initialize();
	void Input(const float& deltaTime);
	void Move(const float& deltaTime);
	std::shared_ptr<TransformComponent> GetPlayerTransform();
	void RenderUI();
	~Player();
};

