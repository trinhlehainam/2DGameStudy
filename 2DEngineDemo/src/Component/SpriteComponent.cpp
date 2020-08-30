#include "SpriteComponent.h"

#include "../GameObject/Entity.h"
#include "TransformComponent.h"
#include "../System/TextureManager.h"
#include "../System/Camera.h"

namespace
{
	constexpr int millisecond_to_second = 1000;
}

SpriteComponent::SpriteComponent(Entity& owner, bool isFixed):Component(owner)
{
	animateUpdate_ = &SpriteComponent::PlayUpdate;
	renderUpdate_ = isFixed ? &SpriteComponent::ScreenFixedRender : &SpriteComponent::NormalRender;
}

void SpriteComponent::Initialize()
{
	transform_ = owner_->GetComponent<TransformComponent>();
	const auto& transform = transform_.lock();
	desRect.pos = transform->pos;
}

void SpriteComponent::AddAnimation(int texture, std::string animID, const Rect& srcRect,
	 const unsigned& animSpeed, const float& rotateSpeed)
{
	Animation anim;
	anim.texture = texture;
	anim.srcRect = srcRect;
	anim.animSpeed = animSpeed;
	anim.rotateSpeed = rotateSpeed;
	TextureManager::GetImageSize(anim.texture, anim.textureW, anim.textureH);
	anim.numCelX = anim.textureW / static_cast<int>(anim.srcRect.w);
	anim.numCelY = anim.textureH / static_cast<int>(anim.srcRect.h);
	anim.indexX = 0;
	anim.indexY = 0;
	animations_.emplace(animID, std::move(anim));
}

void SpriteComponent::SetAnimationOffset(const std::string& animaID, const Vector2& offset)
{
	const auto& transform = transform_.lock();
	auto& animation = animations_.at(animaID);
	animation.offset_ = offset;
	desRect.pos = transform->pos - animation.offset_;
}

void SpriteComponent::Update(const float& deltaTime)
{
	(this->*animateUpdate_)(deltaTime);

	transform_ = owner_->GetComponent<TransformComponent>();
}

void SpriteComponent::PlayUpdate(const float& deltaTime)
{
	auto& animation = animations_.at(currentAnimID);

	speedTimer_ += deltaTime * millisecond_to_second;
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

	const auto& transform = transform_.lock();

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
			/*TextureManager::DrawDebugBox(desRect, 0x00ff00);*/
		}
	}
}

void SpriteComponent::NormalRender()
{
	const auto& transform = transform_.lock();
	auto& animation = animations_.at(currentAnimID);

	desRect.pos = transform->pos - animation.offset_ - Camera::Instance().viewport.pos;
}

void SpriteComponent::ScreenFixedRender()
{
	// Do nothing
}

void SpriteComponent::HaveOffsetRender()
{
	const auto& transform = transform_.lock();
	auto& animation = animations_.at(currentAnimID);

	desRect.pos.X = transform->pos.X - (!isFlipped ? animation.offset_.X :
									desRect.w - transform->w * transform->scale - animation.offset_.X);
	desRect.pos.Y = transform->pos.Y - animation.offset_.Y;
	desRect.pos -= Camera::Instance().viewport.pos;
}

void SpriteComponent::Play(const std::string& animID)
{
	if (IsPlaying(animID)) return;
	currentAnimID = animID;
	animations_.at(animID).indexX = 0;
	animations_.at(animID).indexY = 0;
	speedTimer_ = 0;
	angleRad_ = 0.0f;
	const auto& transform = transform_.lock();
	auto& animation = animations_.at(currentAnimID);
	desRect.w = animation.srcRect.w * transform->scale;
	desRect.h = animation.srcRect.h * transform->scale;
	if (animation.offset_.X != 0.0f || animation.offset_.Y != 0.0f)
		renderUpdate_ = &SpriteComponent::HaveOffsetRender;
	else
		renderUpdate_ = &SpriteComponent::NormalRender;
}

void SpriteComponent::Pause()
{
	animateUpdate_ = &SpriteComponent::StopUpdate;
}

void SpriteComponent::Resume()
{
	animateUpdate_ = &SpriteComponent::PlayUpdate;
}

void SpriteComponent::SetAnimationSpeed(const unsigned int& animSpeed)
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
