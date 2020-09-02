#pragma once
#include <memory>

class Entity;
class GameScene;

class Attack
{
protected:
	std::shared_ptr<Entity> self_;
	std::weak_ptr<Entity> owner_;
	GameScene* gs_;
public:
	Attack(GameScene& gs, const std::shared_ptr<Entity>&);
	virtual ~Attack() = default;
	virtual void Initialize() = 0;
	virtual void SetDamage(const int& damage) = 0;
	virtual int GetDamage() const = 0;
	virtual void Update(const float& deltaTime) = 0;

	bool IsOwnerActive() const;
};

