#include "ShurikenEquip.h"

#include "../../Scene/GameScene.h"
#include "../Entity.h"
#include "../EntityManager.h"
#include "../../Geometry/Geometry.h"
#include "../../Component/TransformComponent.h"
#include "../../Component/SpriteComponent.h"
#include "../../Component/ProjectileEmitterComponent.h"
#include "../../Component/CircleColliderComponent.h"
#include "../../System/CollisionManager.h"

namespace
{
	constexpr unsigned int shuriken_width = 20;
	constexpr unsigned int shuriken_height = 20;
	constexpr unsigned int scale = 1;
	constexpr float move_speed = 700.0f;
	constexpr float move_range = 700.0f;
}

ShurikenEquip::ShurikenEquip(GameScene& gs):Equipment(gs)
{
}

ShurikenEquip::~ShurikenEquip()
{
}

void ShurikenEquip::Attack(const Vector2& startPos, const float& angle)
{
	auto projectile = gs_.entityMng_->AddProjectTile("shuriken");
	projectile->AddComponent<TransformComponent>(startPos, shuriken_width, shuriken_height, scale);
	projectile->AddComponent<SpriteComponent>();
	auto anim = projectile->GetComponent<SpriteComponent>();
	anim->AddAnimation(gs_.GetTexture("shuriken-equip"), "attack",
		Rect(0, 0, shuriken_width, shuriken_height),
		1, 1, 30);
	anim->Play("attack");
	gs_.collisionMng_->AddProjectileCollider(projectile->GetComponent<TransformComponent>(),
		"PLAYER-SHURIKEN", startPos.X , startPos.Y , shuriken_width / 2);

	Vector2 velocity = Vector2(move_speed * cosf(angle), move_speed * sinf(angle));
	projectile->AddComponent<ProjectileEmitterComponent>(startPos, std::move(velocity), move_range);
}

void ShurikenEquip::Initialize()
{

}

void ShurikenEquip::Render()
{
	Equipment::DrawEquipmentBox(gs_.GetTexture("shuriken-icon"));
}
