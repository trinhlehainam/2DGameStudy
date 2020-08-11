#pragma once

#include <string>
#include <memory>

#include "../Geometry/Geometry.h"

class Entity;

class ColliderComponent
{
protected:
	std::weak_ptr<Entity> owner_;
public:
	std::string tag_;
	bool flag_ = false;
	bool IsOwnerExist();
	ColliderComponent(std::shared_ptr<Entity> owner, std::string tag);
	~ColliderComponent() = default;
};

