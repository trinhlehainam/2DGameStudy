#include "MeleeAttack.h"

#include "../../Constant.h"
#include "../../Geometry/Geometry.h"
#include "../../Scene/GameScene.h"

#include "../Entity.h"
#include "../../System/EntityManager.h"
#include "../../System/CollisionManager.h"
#include "../../System/EffectManager.h"
#include "../../System/Time.h"

#include "../../Component/TransformComponent.h"
#include "../../Component/SpriteComponent.h"
#include "../../Component/Attack/MeleeAttackComponent.h"
#include "../../Component/Collider/AABBColliderComponent.h"

MeleeAttack::MeleeAttack(GameScene& gs, const std::shared_ptr<Entity>& owner, const Vector2& startPos,
	const float& w, const float& h, const int& damage):
	Attack(gs,owner)
{
	self_ = gs_->entityMng_->AddProjectTile("bomb");
	self_->AddComponent<TransformComponent>(self_, startPos, w, h);
	auto& collider = gs_->collisionMng_->AddMeleeAttackCollider(self_,
		"PLAYER-BOMB", startPos, w, h);

	self_->AddComponent<MeleeAttackComponent>(self_, damage);
	auto attack = self_->GetComponent<MeleeAttackComponent>();

	// Limit life time of melee attack in just 1 game loop's frame
	auto& time = Time::Instance();
	attack->SetLifeTime(time.MillisecondsPerFrame(time.GetFrameRate()));
}

void MeleeAttack::Initialize()
{
}

void MeleeAttack::SetDamage(const int& damage)
{
	self_->GetComponent<MeleeAttackComponent>()->SetDamage(damage);
}

int MeleeAttack::GetDamage() const
{
	return self_->GetComponent<MeleeAttackComponent>()->GetDamage();;
}

void MeleeAttack::Update(const float& deltaTime)
{

}
