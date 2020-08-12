#pragma once


class Player;
class Vector2;
class GameScene;

class Equipment
{
protected:
	float angle_ = 0.0f;
	GameScene& gs_;

public:
	Equipment(GameScene&);

	virtual ~Equipment() = default;

	virtual void Initialize() = 0;

	// ¡‚Ì•Ší‚ÅUŒ‚‚ğ‰Á‚¦‚é
	virtual void Attack(const Vector2& startPos, const float& angle) = 0;

	virtual void DrawEquipmentBox(int texture);

	virtual void Render() = 0;
};
