#include "TransformComponent.h"

#include "../Constant.h"
#include "../Game Object/Entity.h"
#include "../Geometry/Geometry.h"
#include "../System/Camera.h"

TransformComponent::TransformComponent(Entity& owner, const Vector2& pos, const float& w, const float& h, const float& scale) :Component(owner)
{
	this->pos = pos;
	this->w = w;
	this->h = h;
	this->scale = scale;

}

void TransformComponent::Initialize()
{
}

void TransformComponent::Update(const float& deltaTime)
{
	pos.X = clamp(pos.X, 0.0f, WORLD_MAP_X-w);
}

void TransformComponent::Render()
{

}


