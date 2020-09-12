#include "BornFire.h"

#include "../Scene/GameScene.h"
#include "Entity.h"
#include "../Component/TransformComponent.h"
#include "../Component/SpriteComponent.h"
#include "../Component/Collider/AABBColliderComponent.h"

#include "../System/EntityManager.h"
#include "../System/AssetManager.h"
#include "../System/CollisionManager.h"

namespace
{
	const Vector2 start_pos = Vector2(100.0f, 100.0f);
	const Rect src_rect = Rect(0, 0, 64, 64);
	constexpr float width = 64.0f;
	constexpr float height = 64.0f;
	constexpr float scale = 2.0f;

	constexpr unsigned short int fire_animation_speed = 100;
}

BornFire::BornFire(GameScene& gs):CheckPoint(gs)
{
}

void BornFire::Initialize()
{
	self_ = gs_.entityMng_->AddEntity("born-fire");
	self_->AddComponent<TransformComponent>(self_, start_pos, width, height, scale);
	self_->AddComponent<SpriteComponent>(self_);
	const auto& anim = self_->GetComponent<SpriteComponent>();
	anim->AddAnimation(gs_.assetMng_->GetTexture("born-fire"), "fire", src_rect, fire_animation_speed);
	anim->PlayLoop("fire");
}

void BornFire::Update(const float&)
{
	// born fire is active when Player's rigid body collides with born fire's collider
	isActive_ = collider_->IsCollide();
}

void BornFire::Render()
{
}
