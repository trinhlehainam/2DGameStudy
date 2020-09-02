#include "SwordEquip.h"

#include "../../Scene/GameScene.h"
#include "../../System/CombatManager.h"
#include "../Attack/MeleeAttack.h"

SwordEquip::SwordEquip(GameScene& gs, const std::string& tag, const std::shared_ptr<Entity>& owner, const int& damage):
	Equipment(gs, tag, owner, damage)
{
}

void SwordEquip::Initialize()
{
}

void SwordEquip::Attack(const Vector2& startPos, const float& dirAngle, const float& w, const float& h)
{
	auto melee = gs_.combatMng_->AddAttack<MeleeAttack>(gs_, owner_.lock(), startPos, w, h);
	melee->SetDamage(damage_);
}

void SwordEquip::Render()
{
	Equipment::DrawEquipmentBox(gs_.GetTexture("sword-icon"));
}
