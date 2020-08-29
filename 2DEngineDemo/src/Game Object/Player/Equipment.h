#pragma once
#include <string>

class Player;
class Vector2;
class GameScene;

class Equipment
{
protected:
	float angle_ = 0.0f;
	GameScene& gs_;
	std::string tag_;

public:
	Equipment(GameScene&,const std::string&);

	inline std::string GetTag() const { return tag_; }

	virtual ~Equipment() = default;

	virtual void Initialize() = 0;

	// ¡‚Ì•Ší‚ÅUŒ‚‚ğ‰Á‚¦‚é
	virtual void Attack(const Vector2& startPos, const float& angle) = 0;

	virtual void DrawEquipmentBox(const int& texture) final;

	virtual void Render() = 0;
};
