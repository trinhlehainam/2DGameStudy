#include "ShurikenEquip.h"

#include "../Entity.h"
#include "../../Geometry/Geometry.h"
#include "../../Scene/GameScene.h"

#include "../../Component/TransformComponent.h"
#include "../../Component/SpriteComponent.h"
#include "../../Component/ProjectileEmitterComponent.h"
#include "../../Component/CircleColliderComponent.h"

#include "../../System/EntityManager.h"
#include "../../System/CollisionManager.h"

namespace
{
	constexpr unsigned int shuriken_width = 20;
	constexpr unsigned int shuriken_height = 20;
	constexpr float scale = 1.0f;
	constexpr float shuriken_radius = shuriken_width * scale / 2.0f;
	constexpr float move_speed = 700.0f;
	constexpr float rotate_speed = 60.0f;
	constexpr float move_range = 1000.0f;
	constexpr int shuriken_damage = 1;
}

ShurikenEquip::ShurikenEquip(GameScene& gs,const std::string& tag):Equipment(gs, tag)
{
}

ShurikenEquip::~ShurikenEquip()
{
}

void ShurikenEquip::Attack(const Vector2& startPos, const float& angle)
{
	auto projectile = gs_.entityMng_->AddProjectTile("shuriken");
	projectile->AddComponent<TransformComponent>(projectile, startPos, shuriken_width, shuriken_height, scale);
	projectile->AddComponent<SpriteComponent>(projectile);
	auto anim = projectile->GetComponent<SpriteComponent>();
	anim->AddAnimation(gs_.GetTexture("shuriken-equip"), "attack",
		Rect(0, 0, shuriken_width, shuriken_height),
		1, rotate_speed);
	anim->PlayLoop("attack");
	auto& collider = gs_.collisionMng_->AddProjectileCollider(projectile,
		"PLAYER-SHURIKEN", startPos.X , startPos.Y , shuriken_radius);
	collider.SetOffset(shuriken_radius, shuriken_radius);
	Vector2 velocity = Vector2(move_speed * cosf(angle), move_speed * sinf(angle));
	projectile->AddComponent<ProjectileEmitterComponent>(projectile, startPos, std::move(velocity), move_range, shuriken_damage);
}

void ShurikenEquip::Initialize()
{

}

void ShurikenEquip::Render()
{
	Equipment::DrawEquipmentBox(gs_.GetTexture("shuriken-icon"));
}
