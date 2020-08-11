#pragma once

#include <string>
#include <memory>

#include "../Geometry/Geometry.h"

class TransformComponent;

class ColliderComponent
{
protected:
	std::weak_ptr<TransformComponent> transform_;
public:
	std::string tag_;
	bool flag_ = false;
	ColliderComponent(std::shared_ptr<TransformComponent> transfrom, std::string tag);
	~ColliderComponent() = default;
};

