#include "SpriteComponent.h"

#include <DxLib.h>

#include "../GameObject/Entity.h"
#include "TransformComponent.h"
#include "../System/TextureManager.h"
#include "../System/Camera.h"

SpriteComponent::SpriteComponent(Entity& owner, bool isFixed):Component(owner)
{
	animateUpdate_ = &SpriteComponent::PlayUpdate;
	renderUpdate_ = isFixed ? &SpriteComponent::FixedRender : &SpriteComponent::NormalRender;
}

void SpriteComponent::Initialize()
{
	transform_ = owner_->GetComponent<TransformComponent>();
	auto transform = transform_.lock();
	desRect.pos = transform->pos - offset_;
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

void SpriteComponent::SetOffset(const Vector2& offset)
{
	auto transform = transform_.lock();
	offset_ = offset;
	desRect.pos = transform->pos - offset_;
}

void SpriteComponent::Update(const float& deltaTime)
{
	(this->*animateUpdate_)(deltaTime);

	transform_ = owner_->GetComponent<TransformComponent>();
}

void SpriteComponent::PlayUpdate(const float& deltaTime)
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
}

void SpriteComponent::StopUpdate(const float& deltaTime)
{
	// Do nothing
}

void SpriteComponent::Render()
{
	(this->*renderUpdate_)();

	auto transform = transform_.lock();

	if (desRect.pos.X >= -desRect.w &&
		desRect.pos.X <= Camera::Instance().viewport.w + desRect.w &&
		desRect.pos.Y >= -desRect.h &&
		desRect.pos.Y <= Camera::Instance().viewport.h + desRect.h)
	{
		if (animations_.count(currentAnimID))
		{
			TextureManager::DrawRectRota(animations_.at(currentAnimID).texture, animations_.at(currentAnimID).srcRect,
				desRect, transform->scale, angleRad_, isFlipped);
			// Debug
			/*TextureManager::DrawBox(desRect, 0x00ff00);*/
		}
	}
}

void SpriteComponent::NormalRender()
{
	auto transform = transform_.lock();

	desRect.pos = transform->pos - offset_ - Camera::Instance().viewport.pos;
	desRect.w = transform->w * transform->scale;
	desRect.h = transform->h * transform->scale;
}

void SpriteComponent::FixedRender()
{
	// Do nothing
}

void SpriteComponent::Play(const std::string& animID)
{
	if (IsPlaying(animID)) return;
	currentAnimID = animID;
	animations_.at(animID).indexX = 0;
	animations_.at(animID).indexY = 0;
	speedTimer_ = 0;
	angleRad_ = 0.0f;
}

void SpriteComponent::Pause()
{
	animateUpdate_ = &SpriteComponent::StopUpdate;
}

void SpriteComponent::Resume()
{
	animateUpdate_ = &SpriteComponent::PlayUpdate;
}

void SpriteComponent::SetSpeed(const unsigned int& animSpeed)
{
	auto& animation = animations_.at(currentAnimID);
	animation.animSpeed = animSpeed;
}

bool SpriteComponent::IsPlaying(const std::string& animID)
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
