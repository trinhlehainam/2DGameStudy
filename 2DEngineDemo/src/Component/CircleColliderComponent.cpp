#include "CircleColliderComponent.h"

#include "../Game Object/Entity.h"
#include "TransformComponent.h"
#include "../System/TextureManager.h"
#include "../System/Camera.h"

CircleColliderComponent::CircleColliderComponent(std::shared_ptr<Entity> owner, std::string tag,
	const float& posX, const float& posY, const float& radius):
	ColliderComponent(owner, tag)
{
	collider_.pos = Vector2(posX, posY);
	collider_.radius = radius;
	Initialize();
}

CircleColliderComponent::~CircleColliderComponent()
{
}

void CircleColliderComponent::Initialize()
{
	destCir_.pos = collider_.pos;
	destCir_.radius = collider_.radius;
}

void CircleColliderComponent::Update(const float& deltaTime)
{
	if (owner_.expired())
	{
		return;
	}
	

	auto transform = owner_.lock()->GetComponent<TransformComponent>();
	/*---------------------------NEED-TO-BE-CAUTIOUS-------------------------------*/
	// Because the position of circle's center is different from the Entity postition
	// So it's necessary to move position of circle to center of Entity
	/*------------------------------------------------------------------------------*/
	collider_.pos.X = transform->pos.X + (transform->w * transform->scale) / 2.0f;
	collider_.pos.Y = transform->pos.Y + (transform->h * transform->scale) / 2.0f;
	
}

void CircleColliderComponent::Render()
{
	destCir_.pos = collider_.pos - Camera::Instance().viewport.origin;
	int color = flag_ ? 0xffff00 : 0xff0000;
	/*TextureManager::DrawCircle(destCir_, color);*/
}

