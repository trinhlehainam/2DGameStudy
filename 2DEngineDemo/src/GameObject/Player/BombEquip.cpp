#include "BombEquip.h"

#include "../../Scene/GameScene.h"
#include "../../System/CombatManager.h"
#include "../Attack/BombShot.h"

BombEquip::BombEquip(GameScene& gs, const std::string& tag):Equipment(gs, tag)
{
}

void BombEquip::Initialize()
{
}

void BombEquip::Attack(const Vector2& startPos, const float& angle)
{
	auto bomb = gs_.combatMng_->AddAttack<BombShot>(gs_, startPos, angle);
}

void BombEquip::Render()
{
	Equipment::DrawEquipmentBox(gs_.GetTexture("bomb-icon"));
}
