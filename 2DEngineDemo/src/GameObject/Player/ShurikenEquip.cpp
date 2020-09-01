#include "ShurikenEquip.h"

#include "../../Scene/GameScene.h"
#include "../../System/CombatManager.h"
#include "../Attack/ShurikenShot.h"

ShurikenEquip::ShurikenEquip(GameScene& gs,const std::string& tag):Equipment(gs, tag)
{
}

ShurikenEquip::~ShurikenEquip()
{
}

void ShurikenEquip::Attack(const Vector2& startPos, const float& angle)
{
	auto bomb = gs_.combatMng_->AddAttack<ShurikenShot>(gs_, startPos, angle);
}

void ShurikenEquip::Initialize()
{

}

void ShurikenEquip::Render()
{
	Equipment::DrawEquipmentBox(gs_.GetTexture("shuriken-icon"));
}
