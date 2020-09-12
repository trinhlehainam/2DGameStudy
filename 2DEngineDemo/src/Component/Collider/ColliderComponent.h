#pragma once

#include <string>
#include <memory>

#include "../../Geometry/Geometry.h"
#include "../Component.h"

class Entity;

// Base class for Collider Component
class ColliderComponent:
	public Component
{
protected:
	std::weak_ptr<Entity> owner_;
	bool isActive_ = true;
	bool isCollide_ = false;
	Vector2 offset_ = Vector2(0.0f,0.0f);					// Distance between this collider and owner's position (Transform)
	std::string tag_;
public:
	ColliderComponent(const std::shared_ptr<Entity>& owner, std::string tag);
	ColliderComponent(const std::shared_ptr<Entity>& owner);
	virtual ~ColliderComponent() = default;

	inline std::string GetTag() const { return tag_; }
	inline void SetTag(const std::string& tag) { tag_ = tag; }
	void SetOffset(const float& X, const float& Y);
	inline void Collide() { isCollide_ = true; }
	inline void NoCollide() { isCollide_ = false; }
	inline bool IsCollide() const { return isCollide_; }
	inline bool IsActive() const { return isActive_; }
	inline void Activate() { isActive_ = true; }
	inline void DeActivate() { isActive_ = false; }
};

