#include "SpriteComponent.h"

#include <DxLib.h>

#include "../Game Object/Entity.h"
#include "../System/TextureManager.h"
#include "TransformComponent.h"
#include "../System/Camera.h"

SpriteComponent::SpriteComponent(Entity& owner, bool isFixed):Component(owner),isFixed(isFixed)
{

}

void SpriteComponent::Initialize()
{
	transform_ = owner_->GetComponent<TransformComponent>();
	auto transform = transform_.lock();
	desRect.pos = transform->pos;
	desRect.w = transform->w * transform->scale;
	desRect.h = transform->h * transform->scale;
}

void SpriteComponent::AddAnimation(int texture, std::string animID, const Rect& srcRect,
	 const unsigned& animSpeed, const float& rotateSpeed)
{
	Animation anim;
	anim.texture = texture;
	anim.srcRect = srcRect;
	anim.animSpeed = animSpeed;
	anim.rotateSpeed = rotateSpeed;
	DxLib::GetGraphSize(anim.texture, &anim.textureW, &anim.textureH);
	anim.numCelX = anim.textureW / static_cast<int>(anim.srcRect.w);
	anim.numCelY = anim.textureH / static_cast<int>(anim.srcRect.h);
	anim.indexX = 0;
	anim.indexY = 0;
	animations_.emplace(animID, std::move(anim));
}

void SpriteComponent::Update(const float& deltaTime)
{
	auto& animation = animations_.at(currentAnimID);

	speedTimer_ += deltaTime * 1000.0f;
	animation.indexX = (speedTimer_ / animation.animSpeed) % animation.numCelX;
	if (animation.indexX == animation.numCelX - 1 && animation.numCelX > 1) 
		animation.indexY = (animation.indexY + 1) % animation.numCelY;
	else 
		animation.indexY = (speedTimer_ / animation.animSpeed) % animation.numCelY;

	animation.srcRect.pos.X = animation.indexX * animation.srcRect.w;
	animation.srcRect.pos.Y = animation.indexY * animation.srcRect.w;

	angleRad_ += animation.rotateSpeed * deltaTime;
	transform_ = owner_->GetComponent<TransformComponent>();
}

void SpriteComponent::Render()
{
	auto transform = transform_.lock();
	desRect.pos.X = transform->pos.X - !isFixed * Camera::Instance().viewport.pos.X;
	desRect.pos.Y = transform->pos.Y - !isFixed * Camera::Instance().viewport.pos.Y;
	desRect.w = transform->w * transform->scale;
	desRect.h = transform->h * transform->scale;

	if (desRect.pos.X >= -desRect.w &&
		desRect.pos.X <= Camera::Instance().viewport.w + desRect.w &&
		desRect.pos.Y >= -desRect.h &&
		desRect.pos.Y <= Camera::Instance().viewport.h + desRect.h)
	{
		if (animations_.count(currentAnimID))
		{
			TextureManager::DrawRectRota(animations_.at(currentAnimID).texture, animations_.at(currentAnimID).srcRect,
				desRect, transform->scale, angleRad_, isFlipped);
		}
	}
}

void SpriteComponent::Play(std::string animID)
{
	if (IsPlaying(animID)) return;
	currentAnimID = animID;
	animations_.at(animID).indexX = 0;
	animations_.at(animID).indexY = 0;
	speedTimer_ = 0;
	angleRad_ = 0.0f;
}

bool SpriteComponent::IsPlaying(std::string animID)
{
	return currentAnimID.compare(animID) == 0;
}

bool SpriteComponent::IsFinished()
{
	auto& animation = animations_.at(currentAnimID);
	return animation.indexX == (animation.numCelX - 1) && animation.indexY == (animation.numCelY - 1);
}

SpriteComponent::~SpriteComponent()
{
}
