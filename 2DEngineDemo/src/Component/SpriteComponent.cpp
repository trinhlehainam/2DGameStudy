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
	transform_ = owner_.GetComponent<TransformComponent>();
	auto transform = transform_.lock();
	desRect.origin = transform->pos;
	desRect.w = transform->w * transform->scale;
	desRect.h = transform->h * transform->scale;
}

void SpriteComponent::AddAnimation(int texture, std::string animID, const Rect& srcRect,
	const unsigned int& numCels, const unsigned& animSpeed, const float& rotateSpeed)
{
	Animation anim;
	anim.texture = texture;
	anim.srcRect = srcRect;
	anim.numCels = numCels;
	anim.animSpeed = animSpeed;
	anim.rotateSpeed = rotateSpeed;
	anim.index = 0;
	animations_.emplace(animID, std::move(anim));
}

void SpriteComponent::Update(const float& deltaTime)
{
	auto& animation = animations_.at(currentAnimID);
	speedTimer_ += deltaTime * 1000.0f;
	animation.index = (speedTimer_ / animation.animSpeed) % animation.numCels;
	animation.srcRect.origin.X =  animation.index * animation.srcRect.w;
	angleRad_ += animation.rotateSpeed * deltaTime;

	transform_ = owner_.GetComponent<TransformComponent>();
	
}

void SpriteComponent::Render()
{
	auto transform = transform_.lock();
	desRect.origin.X = transform->pos.X - !isFixed * Camera::Instance().viewport.origin.X;
	desRect.origin.Y = transform->pos.Y - !isFixed * Camera::Instance().viewport.origin.Y;
	desRect.w = transform->w * transform->scale;
	desRect.h = transform->h * transform->scale;

	if (desRect.origin.X >= -desRect.w &&
		desRect.origin.X <= Camera::Instance().viewport.w + desRect.w &&
		desRect.origin.Y >= -desRect.h &&
		desRect.origin.Y <= Camera::Instance().viewport.h + desRect.h)
	{
		if (animations_.count(currentAnimID))
		{
			transform_ = owner_.GetComponent<TransformComponent>();
			auto transform = transform_.lock();
			TextureManager::DrawRectRota(animations_.at(currentAnimID).texture, animations_.at(currentAnimID).srcRect,
				desRect, transform->scale, angleRad_, isFlipped);
		}
	}
}

void SpriteComponent::Play(std::string animID)
{
	if (IsPlaying(animID)) return;
	currentAnimID = animID;
	animations_.at(animID).index = 0;
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
	return animation.index == (animation.numCels - 1);
}


SpriteComponent::~SpriteComponent()
{
	
}
