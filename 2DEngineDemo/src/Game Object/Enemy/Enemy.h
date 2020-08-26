#pragma once

#include <memory>

class GameScene;
class TransformComponent;
class Entity;
class Vector2;

class Enemy
{
protected:
	GameScene& gs_;
	std::weak_ptr<TransformComponent> playerPos_;
	std::shared_ptr<Entity> self_;
public:
	Enemy(GameScene& gs, std::shared_ptr<TransformComponent> playerPos_);
	virtual ~Enemy() = default;
	bool IsOwnerExist();
	virtual void Initialize() = 0;
	virtual void Update(const float& deltaTime) = 0;
	virtual std::unique_ptr<Enemy> MakeClone() = 0;
	virtual void SetPosition(const Vector2& pos) = 0;
};

