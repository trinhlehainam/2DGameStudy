#include "BombEquip.h"

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
	constexpr unsigned int bomb_width = 67;
	constexpr unsigned int bomb_height = 35;
	constexpr float scale = 0.6f;
	constexpr float move_speed = 400.0f;
	constexpr int rotate_speed = 20;
	constexpr float move_range = 700.0f;
}

BombEquip::BombEquip(GameScene& gs):Equipment(gs)
{
}

void BombEquip::Initialize()
{
}

void BombEquip::Attack(const Vector2& startPos, const float& angle)
{
	auto projectile = gs_.entityMng_->AddProjectTile("bomb");
	projectile->AddComponent<TransformComponent>(startPos, bomb_width, bomb_height, scale);
	projectile->AddComponent<SpriteComponent>();
	auto anim = projectile->GetComponent<SpriteComponent>();
	anim->AddAnimation(gs_.GetTexture("bomb-equip"), "attack",
		Rect(0, 0, bomb_width, bomb_height),
		1, 1, rotate_speed);
	anim->Play("attack");
	gs_.collisionMng_->AddProjectileCollider(projectile,
		"PLAYER-SHURIKEN", startPos.X, startPos.Y, bomb_width / 2);

	Vector2 velocity = Vector2(move_speed * cosf(angle), move_speed * sinf(angle));
	projectile->AddComponent<ProjectileEmitterComponent>(startPos, std::move(velocity), move_range);
}

void BombEquip::Render()
{
	Equipment::DrawEquipmentBox(gs_.GetTexture("bomb-icon"));
}
