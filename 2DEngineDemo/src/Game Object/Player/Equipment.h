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

	// ���̕���ōU����������
	virtual void Attack(const Vector2& startPos, const float& angle) = 0;

	virtual void DrawEquipmentBox(const int& texture) final;

	virtual void Render() = 0;
};
