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
	friend class PhysicsManager;
	Vector2 maxVelocity_ = Vector2(700, 700);

	using Update_t = void(RigidBody2D::*)(const float&);
	Update_t updater_;

	void NormalUpdate(const float&);
	void ImpactUpdate(const float&);

	Vector2 impactResist_ = Vector2(1000,1000);
	Vector2 impactVeloctity_ = Vector2(0, 0);
public:
	Rect collider_;
	Rect destRect_;
	Vector2 velocity_ = Vector2(0, 0);
	bool isGrounded_ = false;
	bool isTouchWall_ = false;
	
	void Initialize() override;
	void Update(const float& deltaTime) override;
	void Render() override;
	void SetMaxVelocity(const float& velX, const float& velY);
	void Impact(const Vector2& impactValue);
	
	RigidBody2D(const std::shared_ptr<Entity>& owner, const Vector2& pos, const float&w, const float& h);

	~RigidBody2D() = default;
};

