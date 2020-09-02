#include "SpriteComponent.h"

#include "../Constant.h"
#include "../GameObject/Entity.h"
#include "TransformComponent.h"
#include "../System/TextureManager.h"
#include "../System/Camera.h"

SpriteComponent::SpriteComponent(const std::shared_ptr<Entity>& owner, bool isFixed):Component(owner)
{
	animateUpdate_ = &SpriteComponent::PlayLoopUpdate;
	renderUpdate_ = isFixed ? &SpriteComponent::FixedOnScreenRender : &SpriteComponent::NormalRender;
}

void SpriteComponent::Initialize()
{
	const auto& owner = owner_.lock();
	transform_ = owner->GetComponent<TransformComponent>();
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

int SpriteComponent::GetCurrentCelNO() const
{
	const auto& anim = animations_.at(currentAnimID);
	return (anim.indexY * anim.numCelX + anim.indexX + 1);
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
	playTimer_ += isPlaying_ * deltaTime * second_to_millisecond;
	(this->*animateUpdate_)(deltaTime);
}

void SpriteComponent::PlayLoopUpdate(const float& deltaTime)
{
	auto& animation = animations_.at(currentAnimID);

	animation.indexX = (playTimer_ / animation.animSpeed) % animation.numCelX;
	// Check if the sprite sheet is vertically arranged
	// Condition that sprite sheet is arranged in vertical : sprite sheet has ONLY ONE cell in HORIZON
	if (animation.indexX == animation.numCelX - 1 && animation.numCelX > 1)
		animation.indexY = (animation.indexY + 1) % animation.numCelY;
	else
		animation.indexY = (playTimer_ / animation.animSpeed) % animation.numCelY;

	animation.srcRect.pos.X = animation.indexX * animation.srcRect.w;
	animation.srcRect.pos.Y = animation.indexY * animation.srcRect.w;

	angleRad_ += animation.rotateSpeed * deltaTime;
}

void SpriteComponent::PlayOnceUpdate(const float& deltaTime)
{
	if (IsFinished())
	{
		isPlaying_ = false;
		SetFinish();
		return;
	}

	PlayLoopUpdate(deltaTime);
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

void SpriteComponent::FixedOnScreenRender()
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

void SpriteComponent::PlayAnimation(const std::string& animID)
{
	if (IsPlaying(animID)) return;
	currentAnimID = animID;
	animations_.at(animID).indexX = 0;
	animations_.at(animID).indexY = 0;
	playTimer_ = 0;
	angleRad_ = 0.0f;
	const auto& transform = transform_.lock();
	auto& animation = animations_.at(currentAnimID);
	desRect.w = animation.srcRect.w * transform->scale;
	desRect.h = animation.srcRect.h * transform->scale;
	if (animation.offset_.X != 0.0f || animation.offset_.Y != 0.0f)
		renderUpdate_ = &SpriteComponent::HaveOffsetRender;
	else
		renderUpdate_ = &SpriteComponent::NormalRender;
	isPlaying_ = true;
}

void SpriteComponent::PlayLoop(const std::string& animID)
{
	PlayAnimation(animID);
	animateUpdate_ = &SpriteComponent::PlayLoopUpdate;
	playState_ = PLAY::LOOP;
}

void SpriteComponent::PlayOnce(const std::string& animID)
{
	PlayAnimation(animID);
	animateUpdate_ = &SpriteComponent::PlayOnceUpdate;
	playState_ = PLAY::ONCE;
}

void SpriteComponent::SetFinish()
{
	auto& animation = animations_.at(currentAnimID);
	animation.indexX = (animation.numCelX - 1);
	animation.indexY = (animation.numCelY - 1);
	animation.srcRect.pos.X = animation.indexX * animation.srcRect.w;
	animation.srcRect.pos.Y = animation.indexY * animation.srcRect.w;
	animateUpdate_ = &SpriteComponent::StopUpdate;
}

void SpriteComponent::Pause()
{
	animateUpdate_ = &SpriteComponent::StopUpdate;
	isPlaying_ = false;
}

void SpriteComponent::Resume()
{
	switch (playState_)
	{
		case PLAY::LOOP:
			animateUpdate_ = &SpriteComponent::PlayLoopUpdate;
			break;
		case PLAY::ONCE:
			animateUpdate_ = &SpriteComponent::PlayOnceUpdate;
			break;
	}
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
	return playTimer_ >= animation.animSpeed * (animation.numCelX * animation.numCelY);
	/*return (animation.indexX == (animation.numCelX - 1)) && 
			(animation.indexY == (animation.numCelY - 1));*/
}

SpriteComponent::~SpriteComponent()
{
}
