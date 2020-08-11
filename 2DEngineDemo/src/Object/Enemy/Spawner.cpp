#include "Spawner.h"
#include "../Enemy/Enemy.h"
#include "../Enemy/EnemyManager.h"



Spawner::Spawner(std::unique_ptr<Enemy> protoype, Vector2 pos, EnemyManager& enemyMng):
	prototype_(std::move(protoype)),pos_(pos),enemyMng_(enemyMng)
{
}

void Spawner::CreateClone()
{
	auto clone = std::move(prototype_->MakeClone());
	clone->Initialize();
	clone->SetPosition(pos_);
	enemyMng_.AddEnemy(std::move(clone));
}
