#include "EffectManager.h"

#include "../Scene/GameScene.h"
#include "../Game Object/Entity.h"
#include "../System/EntityManager.h"
#include "../Component/TransformComponent.h"
#include "../Component/SpriteComponent.h"

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

void EffectManager::EmitBloodEffect(const float& posX, const float& posY, bool flipFlag)
{
	auto effect = gs_.entityMng_->AddEffect("emit-blood");
	effect->AddComponent<TransformComponent>(Vector2(posX - 64.0f / 2, posY), 64.0f, 24.0f, 1.0f);
	effect->AddComponent<SpriteComponent>();
	auto anim = effect->GetComponent<SpriteComponent>();
	anim->AddAnimation(gs_.GetTexture("blood"), "emit-blood", Rect(0, 0, 64, 24), 200);
	anim->isFlipped = flipFlag;
	anim->Play("emit-blood");
}

void EffectManager::BombExplosionEffect(const float& posX, const float& posY)
{
	auto effect = gs_.entityMng_->AddEffect("bomb-explosion");
	effect->AddComponent<TransformComponent>(Vector2(posX-48/2.0f, posY), 48, 48, 2.0f);
	effect->AddComponent<SpriteComponent>();
	auto anim = effect->GetComponent<SpriteComponent>();
	anim->AddAnimation(gs_.GetTexture("bomb-explosion"), "explosion", Rect(0, 0, 48, 48), 100);
	anim->Play("explosion");
}

void EffectManager::Render()
{
	for (auto& effect : gs_.entityMng_->effects_)
		effect->Render();
}
