#pragma once

#include <string>
#include <memory>

#include "../Geometry/Geometry.h"
#include "Component.h"

class Entity;

class ColliderComponent:
	public Component
{
protected:
	std::weak_ptr<Entity> owner_;
	bool isActive_ = true;
	bool isHit_ = false;
	Vector2 offset_ = Vector2(0.0f,0.0f);					// Distance between this collider and owner's position (Transform)
public:
	std::string tag_;
	inline void SetColliderTag(const std::string& tag) { tag_ = tag; }
	void SetOffset(const float& X, const float& Y);
	inline bool IsActive() const { return isActive_; }
	inline void Activate() { isActive_ = true; }
	inline void Deactivate() { isActive_ = false; }
	ColliderComponent(const std::shared_ptr<Entity>& owner, std::string tag);
	ColliderComponent(const std::shared_ptr<Entity>& owner);
	virtual ~ColliderComponent() = default;
};

