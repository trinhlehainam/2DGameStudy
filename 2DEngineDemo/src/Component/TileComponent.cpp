#include "TileComponent.h"

#include <DxLib.h>

#include "../System/TextureManager.h"

#include "../System/Camera.h"

TileComponent::TileComponent(const std::shared_ptr<Entity>& owner, int texture, const float& srcX,
	const float& srcY, const float& x,const float& y, const int& tileSize, const float& scale)
	:Component(owner)
{
	this->texture = texture;

	this->srcRect.pos.X = srcX;
	this->srcRect.pos.Y = srcY;
	this->srcRect.w = tileSize;
	this->srcRect.h = tileSize;

	this->desRect.pos = Vector2(x, y);
	this->desRect.w = tileSize * scale;
	this->desRect.h = tileSize * scale;

	this->pos = Vector2(x, y);
}

void TileComponent::Initialize()
{
}

void TileComponent::Update(const float& deltaTime)
{
	
}

void TileComponent::Render()
{
	desRect.pos = pos - Camera::Instance().Position();
	// Render only destination Rectangle is on viewport ( or on the window's screen )
	if (desRect.pos.X >= -desRect.w &&
		desRect.pos.X <= Camera::Instance().ViewWidth() + desRect.w &&
		desRect.pos.Y >= -desRect.h &&
		desRect.pos.Y <= Camera::Instance().ViewHeight() + desRect.h)
	{
		TextureManager::DrawRect(texture, desRect, srcRect);
	}
	
}
