#pragma once

#include <memory>
#include <string>
#include "ColliderComponent.h"

class Entity;

/// <param name="owner"></param>
/// <param name="pos"></param>
/// <param name="w"></param>
/// <param name="h"></param>
/// <returns></returns>
class RigidBody2D:
	public ColliderComponent
{
private:
	friend class CollisionManager;
public:
	Rect collider_;
	Rect destRect_;
	Vector2 velocity_ = Vector2(0, 0);
	bool isGrounded_ = false;
	bool isTouchWall_ = false;

	void Initialize() override;
	void Update(const float& deltaTime) override;
	void Render() override;
	
	RigidBody2D(const std::shared_ptr<Entity>& owner, const Vector2& pos, const float&w, const float& h);

	~RigidBody2D() = default;
};

