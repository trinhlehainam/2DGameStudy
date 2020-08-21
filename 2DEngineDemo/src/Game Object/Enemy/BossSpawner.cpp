#include "BossSpawner.h"

#include "../Enemy/Enemy.h"
#include "../EntityManager.h"
#include "../Enemy/EnemyManager.h"

BossSpawner::BossSpawner(std::unique_ptr<Enemy> protoype, Vector2 pos, EnemyManager& enemyMng) :
	Spawner(std::move(protoype), pos, enemyMng)
{
	updater_ = &BossSpawner::SpawnUpdate;
}

void BossSpawner::Update(const float& deltaTime)
{
	(this->*updater_)(deltaTime);
}

void BossSpawner::SpawnUpdate(const float& deltaTime)
{
	auto clone = std::move(prototype_->MakeClone());
	clone->Initialize();
	clone->SetPosition(pos_);
	enemyMng_.AddEnemy(std::move(clone));
	updater_ = &BossSpawner::SleepUpdate;
}

void BossSpawner::SleepUpdate(const float& deltaTime)
{
}