#include "BombEquip.h"

#include "../../Scene/GameScene.h"
#include "../Entity.h"
#include "../../System/EntityManager.h"
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
	constexpr float bomb_radius = bomb_width * scale / 2.0f;
	constexpr float move_speed = 400.0f;
	constexpr int rotate_speed = 20;
	constexpr float move_range = 700.0f;
	constexpr int bomb_damage = 5;
	constexpr float collider_distance_x = bomb_width * scale / 2.0f;
	constexpr float collider_distance_y = bomb_height * scale / 2.0f;
}

BombEquip::BombEquip(GameScene& gs, const std::string& tag):Equipment(gs, tag)
{
}

void BombEquip::Initialize()
{
}

void BombEquip::Attack(const Vector2& startPos, const float& angle)
{
	auto projectile = gs_.entityMng_->AddProjectTile("bomb");
	projectile->AddComponent<TransformComponent>(projectile, startPos, bomb_width, bomb_height, scale);
	projectile->AddComponent<SpriteComponent>(projectile);
	auto anim = projectile->GetComponent<SpriteComponent>();
	anim->AddAnimation(gs_.GetTexture("bomb-equip"), "attack",
		Rect(0, 0, bomb_width, bomb_height),
		1, rotate_speed);
	anim->PlayLoop("attack");
	auto& collider = gs_.collisionMng_->AddProjectileCollider(projectile,
		"PLAYER-BOMB", startPos.X, startPos.Y, bomb_radius);
	collider.SetOffset(collider_distance_x, collider_distance_y);

	Vector2 velocity = Vector2(move_speed * cosf(angle), move_speed * sinf(angle));
	projectile->AddComponent<ProjectileEmitterComponent>(projectile, startPos, std::move(velocity), move_range, bomb_damage);
}

void BombEquip::Render()
{
	Equipment::DrawEquipmentBox(gs_.GetTexture("bomb-icon"));
}
