#include "TileComponent.h"

#include <DxLib.h>

#include "../System/TextureManager.h"

#include "../System/Camera.h"

TileComponent::TileComponent(Entity& owner, int texture, const float& srcX, 
	const float& srcY, const float& x,const float& y, const int& tileSize, const float& scale):
	Component(owner)
{
	this->texture = texture;

	this->srcRect.origin.X = srcX;
	this->srcRect.origin.Y = srcY;
	this->srcRect.w = tileSize;
	this->srcRect.h = tileSize;

	this->desRect.origin = Vector2(x, y);
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
	desRect.origin.X = pos.X - Camera::Instance().viewport.origin.X;
	desRect.origin.Y = pos.Y - Camera::Instance().viewport.origin.Y;
	// Render only destination Rectangle is on viewport ( or on the window's screen )
	if (desRect.origin.X >= -desRect.w &&
		desRect.origin.X <= Camera::Instance().viewport.w + desRect.w &&
		desRect.origin.Y >= -desRect.h &&
		desRect.origin.Y <= Camera::Instance().viewport.h + desRect.h)
	{
		TextureManager::DrawRect(texture, desRect, srcRect);
	}
	
}
