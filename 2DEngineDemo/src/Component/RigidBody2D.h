#pragma once

#include <memory>
#include "../Geometry/Geometry.h"

class TransformComponent;

class RigidBody2D
{
public:
	std::weak_ptr<TransformComponent> transform_;
	Rect collider_;
	Vector2 velocity_ = Vector2(100, 100);
	Rect desRect_;
	bool flag_ = false;
	bool isGrounded_ = false;

	void SetVelocity(const Vector2& velocity);
	void Update(const float& deltaTime);
	void Render();
	
	RigidBody2D(std::shared_ptr<TransformComponent> transform, const Vector2& pos, const float&w, const float& h);
	~RigidBody2D() = default;
};

