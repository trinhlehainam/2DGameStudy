#pragma once
#include "Enemy.h"

class RigidBody2D;

class Slasher:
	public Enemy
{
private:
	using ActionUpdate_t = void (Slasher::*)(const float& deltaTime);
	ActionUpdate_t actionUpdate_;
	std::shared_ptr<RigidBody2D> rigidBody_;

	float timer_ = 0.0f;

	void AimPlayer(const float& deltaTime);
	void SlashUpdate(const float& deltaTime);
	void HurtUpdate(const float& deltaTime);
	void DeathUpdate(const float& deltaTime);
	void WaitDestroyUpdate(const float& deltaTime);

	void CheckHit();
public:
	Slasher(GameScene& gs, const std::shared_ptr<TransformComponent>& playerPos);

	~Slasher();

	void Update(const float& deltaTime) override;
	void Initialize() override;
	std::unique_ptr<Enemy> MakeClone();
	void SetPosition(const Vector2& pos);
};

