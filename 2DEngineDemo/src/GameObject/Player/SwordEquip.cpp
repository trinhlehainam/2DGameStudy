#include "SwordEquip.h"

#include "../Entity.h"
#include "../../Geometry/Geometry.h"
#include "../../Scene/GameScene.h"

#include "../../Component/TransformComponent.h"
#include "../../Component/SpriteComponent.h"

#include "../../System/EntityManager.h"
#include "../../System/CollisionManager.h"

SwordEquip::SwordEquip(GameScene& gs, const std::string& tag):Equipment(gs, tag)
{
}

void SwordEquip::Initialize()
{
}

void SwordEquip::Attack(const Vector2& startPos, const float& angle)
{
}

void SwordEquip::Render()
{
	Equipment::DrawEquipmentBox(gs_.GetTexture("sword-icon"));
}
