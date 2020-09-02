#include "BombEquip.h"

#include "../../Scene/GameScene.h"
#include "../../System/CombatManager.h"
#include "../Attack/BombShot.h"

BombEquip::BombEquip(GameScene& gs, const std::string& tag, const std::shared_ptr<Entity>& owner, const int& damage):
	Equipment(gs, tag, owner, damage)
{
}

void BombEquip::Initialize()
{
}

void BombEquip::Attack(const Vector2& startPos, const float& dirAngle, const float& w, const float& h)
{
	auto bomb = gs_.combatMng_->AddAttack<BombShot>(gs_, owner_.lock(), startPos, dirAngle);
	bomb->SetDamage(damage_);
}

void BombEquip::Render()
{
	Equipment::DrawEquipmentBox(gs_.GetTexture("bomb-icon"));
}
