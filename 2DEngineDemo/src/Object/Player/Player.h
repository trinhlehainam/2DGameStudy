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
	FALL
};

class Player
{
private:
	
	GameScene& gs_;
	RigidBody2D* rigidBody_;
	std::unique_ptr<KeyboardInput> input_;
	std::shared_ptr<Entity> self_;
	MOVE moveState_ = MOVE::IDLE;

	using UpdateStateFunc_t = void (Player::*)();

	std::vector<std::unique_ptr<Equipment>> equipments_;
	int currentEquip_ = 0;
	float attackAngle_ = 0.0f;
	
	void SetAngleDirection();
public:
	void UpdateState();
	Player(GameScene& gs);
	void Initialize();
	void Input(const float& deltaTime);
	TransformComponent& GetPlayerTransform();
	void RenderUI();
	~Player();
};

