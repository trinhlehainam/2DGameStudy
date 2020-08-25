#pragma once

#include <memory>

class GameScene;
class TransformComponent;
class Entity;
class RigidBody2D;
class Vector2;

class Enemy
{
protected:
	GameScene& gs_;
	TransformComponent& playerPos_;
	std::weak_ptr<Entity> self_;
public:
	Enemy(GameScene& gs, TransformComponent& playerPos_);
	virtual ~Enemy() = default;
	bool IsOwnerExist();
	virtual void Initialize() = 0;
	virtual void Update(const float& deltaTime) = 0;
	virtual std::unique_ptr<Enemy> MakeClone() = 0;
	virtual void SetPosition(const Vector2& pos) = 0;
};

