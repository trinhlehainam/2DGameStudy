#pragma once
#include "Enemy.h"

class RigidBody2D;

class Slasher:
	public Enemy
{
private:
	using ActionUpdate_t = void (Slasher::*)(const float& deltaTime);
	ActionUpdate_t actionUpdate;
	std::shared_ptr<RigidBody2D> rigidBody_;

	void AimPlayer(const float& deltaTime);
	void SlashUpdate(const float& deltaTime);
public:
	Slasher(GameScene& gs, std::shared_ptr<TransformComponent> playerPos);

	~Slasher();

	void Update(const float& deltaTime) override;
	void Initialize() override;
	std::unique_ptr<Enemy> MakeClone();
	void SetPosition(const Vector2& pos);
};

