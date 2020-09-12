#pragma once

#include <memory>
#include "../Geometry/Geometry.h"

class Entity;
class GameScene;
class AABBColliderComponent;

class CheckPoint
{
protected:
	GameScene& gs_;
	bool isActive_ = false;
	bool isSelected_ = false;
	std::shared_ptr<Entity> self_;
	std::shared_ptr<AABBColliderComponent> collider_;
public:
	CheckPoint(GameScene& gs);
	virtual ~CheckPoint() = default;

	virtual void Initialize() = 0;
	virtual void Update(const float&) = 0;
	virtual void Render() = 0;

	Vector2 Position() const;
	float Width() const;
	float Height() const;
	void SetPosition(const Vector2& pos);
	inline bool IsSelected() const { return isSelected_; }
	inline void Select() { isSelected_ = true; }
	inline void DeSelect() { isSelected_ = false; }
	inline bool IsActive() const { return isActive_; }
	inline void Activate() { isActive_ = true; }
	inline void DeActivate() { isActive_ = false; }
};

