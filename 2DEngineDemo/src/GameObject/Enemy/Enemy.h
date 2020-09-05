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
	std::weak_ptr<TransformComponent> playerTransform_;
	std::shared_ptr<Entity> self_;
public:
	Enemy(GameScene& gs, const std::shared_ptr<TransformComponent>& playerPos_);
	virtual ~Enemy() = default;
	bool IsOwnerExist() const;
	virtual void Initialize() = 0;
	virtual void Update(const float& deltaTime) = 0;
	virtual std::unique_ptr<Enemy> MakeClone() = 0;
	virtual void SetPosition(const Vector2& pos) = 0;
};

