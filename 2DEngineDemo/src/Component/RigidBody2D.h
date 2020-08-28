#pragma once

#include <memory>
#include <string>
#include "../Geometry/Geometry.h"

class Entity;

/// <param name="owner"></param>
/// <param name="pos"></param>
/// <param name="w"></param>
/// <param name="h"></param>
/// <returns></returns>
class RigidBody2D
{
private:
	friend class CollisionManager;
	std::weak_ptr<Entity> owner_;
public:
	Rect collider_;
	Vector2 velocity_ = Vector2(0, 0);
	Rect desRect_;
	bool isActive_ = true;
	bool isGrounded_ = false;
	std::string tag_;

	void Update(const float& deltaTime);
	void Render();
	bool IsOwnerExist();
	
	RigidBody2D(std::shared_ptr<Entity> owner, const Vector2& pos, const float&w, const float& h);

	~RigidBody2D() = default;
};

