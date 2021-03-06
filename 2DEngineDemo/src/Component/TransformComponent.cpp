#include "TransformComponent.h"

#include "../Constant.h"
#include "../GameObject/Entity.h"
#include "../Geometry/Geometry.h"
#include "../System/Camera.h"

TransformComponent::TransformComponent(const std::shared_ptr<Entity>& owner, const Vector2& pos, const float& w,
	const float& h, const float& scale):Component(owner)
{
	this->pos = pos;
	this->w = w;
	this->h = h;
	this->scale = scale;
	leftLimit_ = 0.0f;
	rightLimit_ = WORLD_MAP_X - w;
}

void TransformComponent::SetLeftLimit(const float& limit)
{
	leftLimit_ = limit;
}

void TransformComponent::SetRightLimit(const float& limit)
{
	rightLimit_ = limit;
}

void TransformComponent::Initialize()
{
}

void TransformComponent::Update(const float& deltaTime)
{
	
}

void TransformComponent::UpdateLimitPosition(float& posX, float& posY)
{
	posX = clamp(posX, leftLimit_, rightLimit_);
}

void TransformComponent::Render()
{
}


