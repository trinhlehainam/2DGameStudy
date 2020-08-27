#pragma once

#include <string>
#include <memory>

#include "../Geometry/Geometry.h"

class Entity;

class ColliderComponent
{
protected:
	std::weak_ptr<Entity> owner_;
	bool isActive_ = true;
	bool isHit_ = false;
	Vector2 distance_ = Vector2(0.0f,0.0f);					// Distance between this collider and owner's position (Transform)
public:
	std::string tag_;
	bool flag_ = false;
	bool IsOwnerExist();
	std::string OwnerTag();
	void SetDistance(const float& X, const float& Y);
	ColliderComponent(std::shared_ptr<Entity> owner, std::string tag);
	~ColliderComponent() = default;
};

