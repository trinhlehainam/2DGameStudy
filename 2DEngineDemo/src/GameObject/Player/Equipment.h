#pragma once
#include <string>
#include <memory>

class Player;
class GameScene;
class Entity;
class Vector2;

class Equipment
{
protected:
	float angle_ = 0.0f;
	GameScene& gs_;
	std::string tag_;
	std::weak_ptr<Entity> owner_;
	int damage_;
public:
	Equipment(GameScene&, const std::string&, const std::shared_ptr<Entity>&, const int&);
	virtual ~Equipment() = default;

	virtual void Initialize() = 0;
	// ¡‚Ì•Ší‚ÅUŒ‚‚ğ‰Á‚¦‚é
	virtual void Attack(const Vector2& startPos, const float& dirAngle = 0, const float& w = 0, const float& h = 0) = 0;
	virtual void DrawEquipmentBox(const int& texture) final;
	virtual void Render() = 0;

	inline std::string GetTag() const { return tag_; }
	inline void SetDamage(const int& damage) { damage_ = damage; }
	inline int GetDamage() const { return damage_; }
};
