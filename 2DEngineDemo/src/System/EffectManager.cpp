#include "EffectManager.h"

#include "../Scene/GameScene.h"
#include "../GameObject/Entity.h"
#include "../System/EntityManager.h"
#include "../Component/TransformComponent.h"
#include "../Component/SpriteComponent.h"

namespace
{
	// Infor for Emitting Blood effect
	constexpr float blood_width = 64.0f;
	constexpr float blood_height = 24.0f;
	constexpr unsigned int blood_anim_speed = 200;

	// Infor for Bomb Explosion effect
	constexpr float bomb_exp_width = 48.0f;
	constexpr float bomb_exp_height = 48.0f;
	constexpr unsigned int bomb_exp_anim_speed = 100;
}

EffectManager::EffectManager(GameScene& gs):gs_(gs)
{
}

void EffectManager::Update(const float& deltaTime)
{
	for (auto& effect : gs_.entityMng_->effects_)
	{
		auto anim = effect->GetComponent<SpriteComponent>();
		if (anim->IsFinished())
		{
			effect->Destroy();
			continue;
		}
		effect->Update(deltaTime);
	}	
}

void EffectManager::EmitBloodEffect(const float& posX, const float& posY, bool flipFlag, const float& scale)
{
	auto effect = gs_.entityMng_->AddEffect("emit-blood");
	float blood_offset_x = blood_width * scale / 2.0f;
	float blood_offset_y = blood_height * scale / 2.0f;
	Vector2 pos = flipFlag ? Vector2(posX, posY - blood_offset_y) :
							 Vector2(posX - blood_width * scale, posY - blood_offset_y);
	effect->AddComponent<TransformComponent>(effect, pos, blood_width, blood_height, scale);
	effect->AddComponent<SpriteComponent>(effect);
	auto anim = effect->GetComponent<SpriteComponent>();
	anim->AddAnimation(gs_.GetTexture("blood"), "emit-blood", Rect(0, 0, blood_width, blood_height), blood_anim_speed);
	anim->isFlipped = flipFlag;
	anim->PlayOnce("emit-blood");
}

void EffectManager::BombExplosionEffect(const float& posX, const float& posY, const float& scale)
{
	auto effect = gs_.entityMng_->AddEffect("bomb-explosion");
	float bomb_exp_offset_x = bomb_exp_width * scale / 2.0f;
	float bomb_exp_offset_y = bomb_exp_height * scale / 2.0f;
	effect->AddComponent<TransformComponent>(effect, Vector2(posX - bomb_exp_offset_x, posY - bomb_exp_offset_y), bomb_exp_width, bomb_exp_height, scale);
	effect->AddComponent<SpriteComponent>(effect);
	auto anim = effect->GetComponent<SpriteComponent>();
	anim->AddAnimation(gs_.GetTexture("bomb-explosion"), "explosion", Rect(0, 0, bomb_exp_width, bomb_exp_height), bomb_exp_anim_speed);
	anim->PlayOnce("explosion");
}

void EffectManager::Render()
{
	for (auto& effect : gs_.entityMng_->effects_)
		effect->Render();
}
