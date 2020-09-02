#include "ShurikenEquip.h"

#include "../../Scene/GameScene.h"
#include "../../System/CombatManager.h"
#include "../Attack/ShurikenShot.h"

ShurikenEquip::ShurikenEquip(GameScene& gs,const std::string& tag, const std::shared_ptr<Entity>& owner, const int& damage):
	Equipment(gs, tag, owner, damage)
{
}

ShurikenEquip::~ShurikenEquip()
{
}

void ShurikenEquip::Attack(const Vector2& startPos, const float& dirAngle, const float& w, const float& h)
{
	auto shuriken = gs_.combatMng_->AddAttack<ShurikenShot>(gs_, owner_.lock(), startPos, dirAngle);
	shuriken->SetDamage(damage_);
}

void ShurikenEquip::Initialize()
{

}

void ShurikenEquip::Render()
{
	Equipment::DrawEquipmentBox(gs_.GetTexture("shuriken-icon"));
}

