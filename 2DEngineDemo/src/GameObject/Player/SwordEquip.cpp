#include "SwordEquip.h"

#include "../Entity.h"
#include "../../Scene/GameScene.h"

#include "../../Component/TransformComponent.h"
#include "../../Component/SpriteComponent.h"
#include "../../Component/Collider/AABBColliderComponent.h"

#include "../../System/EntityManager.h"
#include "../../System/CollisionManager.h"

SwordEquip::SwordEquip(GameScene& gs, const std::string& tag, const std::shared_ptr<Entity>& owner, const int& damage):
	Equipment(gs, tag, owner, damage)
{
}

void SwordEquip::Initialize()
{
}

void SwordEquip::Attack(const Vector2& startPos, const float& angle, const float& w, const float& h)
{
	
}

void SwordEquip::Render()
{
	Equipment::DrawEquipmentBox(gs_.GetTexture("sword-icon"));
}
